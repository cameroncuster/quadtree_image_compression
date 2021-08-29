#define OLC_PGE_APPLICATION
#include "application.h"

void usage()
{
   std::cout << "quadTree filename.png threshold[0 - 127]" << std::endl;
}

bool checkParameters(int argc, char **argv, int &threshold)
{
   if (argc != 3)
   {
      usage();
      return false;
   }
   std::ifstream ifile;
   ifile.open(argv[1]);
   if (!ifile)
   {
      usage();
      return false;
   }

   try
   {
      threshold = stoi(argv[2]);
   }
   catch (...)
   {
      usage();
      return false;
   }

   if( threshold < 0 || threshold > 128 )
   {
       usage();
       return false;
   }
   return true;
}


int main(int argc, char *argv[])
{
   int threshold;
   if (!checkParameters(argc, argv, threshold))
      return -1;

   Application demo(argv[1], threshold);

   cout << "Width: " << demo.Width() << " Height: " << demo.Height() << endl;
   if (demo.Construct(demo.Width() * 2, demo.Height(), 1, 1))
      demo.Start();

   return 0;
}
