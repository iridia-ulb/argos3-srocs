/**
* @file <argos3/plugins/robots/drone/hardware/main.cpp>
*
* @author Michael Allwright - <allsey87@gmail.com>
*/

#include <sys/time.h>

#include <csignal>

#include <argos3/core/utility/configuration/command_line_arg_parser.h>
#include <argos3/core/utility/plugins/dynamic_loading.h>
#include <argos3/plugins/robots/drone/hardware/drone.h>

using namespace argos;

struct SScript {
   SScript(const std::string& str_path,
           const std::string& str_arguments) :
      Path(str_path),
      Arguments(str_arguments) {}
   std::string Path;
   std::string Arguments;
};

void RunScripts(const std::vector<SScript>& vec_scripts) {
   for(const SScript& s_script : vec_scripts) {
      int nStatus = 
         std::system((s_script.Path + " " + s_script.Arguments).c_str());
      if(WEXITSTATUS(nStatus) != 0) {
         THROW_ARGOSEXCEPTION("script \"" << s_script.Path << "\" failed.");
      }
   }
}

void handler(int n_signal) {
  CDrone::GetInstance().SetSignal(n_signal);
  /* allow the user to kill application immediately with ctrl-c */
  std::signal(SIGINT, SIG_DFL);
  /* ignore other signals and attempt to allow the robot to shutdown */
  std::signal(SIGABRT, SIG_IGN);
  std::signal(SIGFPE, SIG_IGN);
  std::signal(SIGILL, SIG_IGN);
  std::signal(SIGSEGV, SIG_IGN);
  std::signal(SIGTERM, SIG_IGN);
  std::signal(SIGPIPE, SIG_IGN);
}

int main(int n_argc, char** ppch_argv) {
   LOG.DisableColoredOutput();
   LOGERR.DisableColoredOutput();
   /* On exit, flush the logs */
   std::atexit([] {
      LOG.Flush();
      LOGERR.Flush();
   });
   /* capture signals */
   std::signal(SIGINT, handler);
   std::signal(SIGABRT, handler);
   std::signal(SIGFPE, handler);
   std::signal(SIGILL, handler);
   std::signal(SIGSEGV, handler);
   std::signal(SIGTERM, handler);
   std::signal(SIGPIPE, handler);
   try {
      bool bUsageHelp;
      std::string strConfigurationFile;
      std::string strRouterAddr;
      std::string strControllerId;
      std::string strPixhawkConf;
      std::string strSensorDataPath;
      CCommandLineArgParser cCommandLineArgParser;
      cCommandLineArgParser.AddFlag('h',
                    "help",
                    "display this usage information",
                    bUsageHelp);
      cCommandLineArgParser.AddArgument<std::string>('c',
                                                     "config",
                                                     "the configuration file [REQUIRED]",
                                                     strConfigurationFile);
      cCommandLineArgParser.AddArgument<std::string>('i',
                                                     "id",
                                                     "the ID to use for the controller (defaults to hostname)",
                                                     strControllerId);
      cCommandLineArgParser.AddArgument<std::string>('p',
                                                     "pixhawk",
                                                     "the device and baud rate to use for the Pixhawk",
                                                     strPixhawkConf);
      cCommandLineArgParser.AddArgument<std::string>('r',
                                                     "router",
                                                     "the address and port of the message router",
                                                     strRouterAddr);
      cCommandLineArgParser.AddArgument<std::string>('s',
                                                     "sensor-data-path",
                                                     "path to where sensor data should be saved",
                                                     strSensorDataPath);
      /* Parse command line */
      cCommandLineArgParser.Parse(n_argc, ppch_argv);
      if(bUsageHelp) {
         cCommandLineArgParser.PrintUsage(LOG);
         return EXIT_SUCCESS;
      } 
      else {
         if (strConfigurationFile.empty()) {
            THROW_ARGOSEXCEPTION("configuration file not provided");
         }
         if (strControllerId.empty()) {
            /* no controller id provided */
            char pchBuffer[64];
            if (::gethostname(pchBuffer, 64) == 0) {
               strControllerId.assign(pchBuffer);
               LOG << "[INFO] Controller ID not provided, using \""
                   << strControllerId << "\"" << std::endl;
            }
            else {
               THROW_ARGOSEXCEPTION("Could not get hostname for controller ID");
            }
         }
      }
      /* initialize the RNG */
      CRandom::CreateCategory("argos", 0);
      /* load all libraries */
      CDynamicLoading::LoadAllLibraries();
      /* create the configuration with the provided file */
      ticpp::Document tConfig = ticpp::Document(strConfigurationFile);
      /* load the file */
      tConfig.LoadFile();
      TConfigurationNode& tConfiguration = *tConfig.FirstChildElement();
      /* select the first controller */
      TConfigurationNode& tControllers = GetNode(tConfiguration, "controllers");
      TConfigurationNodeIterator itController;
      itController = itController.begin(&tControllers);
      if(itController == nullptr) {
         THROW_ARGOSEXCEPTION("Configuration file does not declare a controller");
      }
      /* get the framework node */
      TConfigurationNode& tFramework = GetNode(tConfiguration, "framework");
      /* parse the scripts */
      std::vector<SScript> m_vecPreInitScripts;
      std::vector<SScript> m_vecPostInitScripts;
      std::vector<SScript> m_vecPreDestroyScripts;
      std::vector<SScript> m_vecPostDestroyScripts;
      TConfigurationNodeIterator itScript("script");
      for(itScript = itScript.begin(&tFramework);
          itScript != itScript.end();
          ++itScript) {
         std::string strPath;
         std::string strEvent;
         std::string strArguments;
         GetNodeAttribute(*itScript, "path", strPath);
         GetNodeAttribute(*itScript, "event", strEvent);
         GetNodeAttributeOrDefault(*itScript, "arguments", strArguments, strArguments);
         if(strEvent == "pre_init") {
            m_vecPreInitScripts.emplace_back(strPath, strArguments);
         }
         else if(strEvent == "post_init") {
            m_vecPostInitScripts.emplace_back(strPath, strArguments);
         }
         else if(strEvent == "pre_destroy") {
            m_vecPreDestroyScripts.emplace_back(strPath, strArguments);
         }
         else if(strEvent == "post_destroy") {
            m_vecPostDestroyScripts.emplace_back(strPath, strArguments);
         }
         else {
            THROW_ARGOSEXCEPTION("Unsupported event for running script");
         }
      }
      /* get the experiment node */
      TConfigurationNode& tExperiment = GetNode(tFramework, "experiment");
      /* get the target tick length */
      UInt32 unTicksPerSec = 0;
      GetNodeAttribute(tExperiment, "ticks_per_second", unTicksPerSec);
      /* get the target number of ticks */
      UInt32 unLength = 0;
      GetNodeAttributeOrDefault(tExperiment, "length", unLength, unLength);
      /* get Drone instance */
      CDrone& cDrone = CDrone::GetInstance();
      /* initialize the drone */
      RunScripts(m_vecPreInitScripts);
      cDrone.Init(*itController,
                   strControllerId,
                   strRouterAddr,
                   strPixhawkConf,
                   strSensorDataPath,
                   unTicksPerSec,
                   unLength);
      RunScripts(m_vecPostInitScripts);
      /* start the Drone's main loop */
      cDrone.Execute();
      /* clean up */
      RunScripts(m_vecPreDestroyScripts);
      cDrone.Destroy();
      RunScripts(m_vecPostDestroyScripts);
      /* load all libraries */
      CDynamicLoading::UnloadAllLibraries();
      /* uninitialize the RNG */
      CRandom::RemoveCategory("argos");
   } catch (CARGoSException& ex) {
      LOGERR << ex.what() << std::endl;
      return EXIT_FAILURE;
   }
   /* exit */
   return EXIT_SUCCESS;
}

