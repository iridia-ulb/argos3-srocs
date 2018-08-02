/**
* @file <argos3/plugins/robots/builderbot/hardware/main.cpp>
*
* @author Michael Allwright - <allsey87@gmail.com>
*/

#include <csignal>

#include <argos3/core/utility/configuration/command_line_arg_parser.h>
#include <argos3/core/utility/plugins/dynamic_loading.h>
#include <argos3/plugins/robots/builderbot/hardware/builderbot.h>

using namespace argos;

void handler(int n_signal) {
  CBuilderBot::GetInstance().SetSignal(n_signal);
  /* allow the user to kill application immediately with ctrl-c */
  std::signal(SIGINT, SIG_DFL);
  /* ignore other signals and attempt to allow the robot to shutdown */
  std::signal(SIGABRT, SIG_IGN);
  std::signal(SIGFPE, SIG_IGN);
  std::signal(SIGILL, SIG_IGN);
  std::signal(SIGSEGV, SIG_IGN);
  std::signal(SIGTERM, SIG_IGN);
}

int main(int n_argc, char** ppch_argv) {
   /* On exit, flush the log */
   std::atexit([] {
      LOGERR.Flush();
      LOG.Flush();
   });
   /* capture signals */
   std::signal(SIGINT, handler);
   std::signal(SIGABRT, handler);
   std::signal(SIGFPE, handler);
   std::signal(SIGILL, handler);
   std::signal(SIGSEGV, handler);
   std::signal(SIGTERM, handler);
   /* The name of the configuration file */
   std::string strConfigurationFile;
   std::string strControllerId;

   try {
      /* Parse command line */
      bool bUsageHelp;
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
                                                     "controller",
                                                     "the controller identifier",
                                                     strControllerId);     
      cCommandLineArgParser.Parse(n_argc, ppch_argv);
      if(bUsageHelp) {
         /* if -h or --help specified, print help descr and nothing else */
         cCommandLineArgParser.PrintUsage(LOG);
         LOG.Flush();
         return EXIT_SUCCESS;
      } 
      else {
         /* if no -h or --help specified */
         /* be sure to have config file set */
         if (strConfigurationFile.empty()) {
            THROW_ARGOSEXCEPTION("configuration file not provided");
         }
      } /* else all is ok, we can continue */
   } catch (CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error while parsing arguments", ex);
   }
   /* Load all libraries */
   CDynamicLoading::LoadAllLibraries();
   /* Create the configuration with the provided file */
   ticpp::Document tConfiguration = ticpp::Document(strConfigurationFile);
   /* Load the file */
   tConfiguration.LoadFile();
   /* Get robot instance */
   CBuilderBot& cBuilderBot = CBuilderBot::GetInstance();
   /* Initialize the BuilderBot */
   cBuilderBot.Init(*tConfiguration.FirstChildElement(), strControllerId);
   /* Start the BuilderBot's main loop */
   cBuilderBot.Execute();
   /* Clean up resources */
   cBuilderBot.Destroy();
   /* Load all libraries */
   CDynamicLoading::UnloadAllLibraries();
   /* Exit */
   return EXIT_SUCCESS;
}

