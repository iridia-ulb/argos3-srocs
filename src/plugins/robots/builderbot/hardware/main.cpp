/**
* @file <argos3/plugins/robots/builderbot/hardware/main.cpp>
*
* @author Michael Allwright - <allsey87@gmail.com>
*/

#include <argos3/core/utility/configuration/command_line_arg_parser.h>
#include <argos3/plugins/robots/builderbot/hardware/builderbot.h>

using namespace argos;

int main(int n_argc, char** ppch_argv) {
   /* On exit, flush the log */
   ::atexit([] {
      LOG.Flush();
   });
   /* Get robot instance */
   CBuilderBot& cBuilderBot = CBuilderBot::GetInstance();
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
                                                     "the controller identifier [REQUIRED]",
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
         if (strControllerId.empty()) {
            THROW_ARGOSEXCEPTION("controller identifier not provided");
         }

      } /* else all is ok, we can continue */
   } catch (CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error while parsing arguments", ex);
   }
   /* Create the configuration with the provided file */
   ticpp::Document tConfiguration = ticpp::Document(strConfigurationFile);
   /* Load the file */
   tConfiguration.LoadFile();
   /* Initialize the BuilderBot */
   cBuilderBot.Init(*tConfiguration.FirstChildElement(), strControllerId);
   /* Start the BuilderBot's main loop */
   cBuilderBot.Execute();
   /* Clean up resources */
   cBuilderBot.Destroy();
   /* Exit */
   return EXIT_SUCCESS;
}

