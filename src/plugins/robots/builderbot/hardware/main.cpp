/**
 * @file <argos3/plugins/robots/builderbot/hardware/main.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include <argos3/core/utility/configuration/command_line_arg_parser.h>

using namespace argos;

int main(int argc, char** argv) {
   /* The name of the XML config file */
   std::string strConfigFileName;
   /* The id of the controller to use (as specified in the XML config file) */
   std::string strControllerId;
   try {
      /*
       * Parse command line
       */
      bool bUsageHelp;
      CCommandLineArgParser cCLAP;
      cCLAP.AddFlag('h',
                    "help",
                    "display this usage information",
                    bUsageHelp);
      cCLAP.AddArgument<std::string>('c',
                   "configuration-file",
                   "the experiment configuration file [REQUIRED]",
                   strConfigFileName);
      cCLAP.AddArgument<std::string>('i',
                   "lua-controller",
                   "the lua controller you want to use [REQUIRED]",
                   strControllerId);
      cCLAP.Parse(argc, argv);
      if(bUsageHelp) {
         /* if -h or --help specified, print help descr and nothing else */
         cCLAP.PrintUsage(LOG);
         LOG.Flush();
         return EXIT_SUCCESS;
      } else {
         /* if no -h or --help specified */
         /* be sure to have config file set */
         if (strConfigFileName.empty()) {
            THROW_ARGOSEXCEPTION("No config file given, see help (-h or --help)");
         }
         /* AND controller id set */
         if (strControllerId.empty()) {
            THROW_ARGOSEXCEPTION("No controller id given, see help (-h or --help)");
         }
      } /* else all is ok, we can continue */
   } catch (CARGoSException& ex) {
      LOGERR << "[FATAL] Error while parsing args"
      << std::endl
      << ex.what()
      << std::endl;
      LOGERR.Flush();
      return EXIT_FAILURE;
   }
   /* Normal ending conditions */
   LOG << "strConfigFileName = " << strConfigFileName << std::endl;
   LOG << "strControllerId = " << strControllerId << std::endl;
   LOG << "[INFO] Controller terminated" << std::endl;
   LOG.Flush();
   return EXIT_SUCCESS;
}
