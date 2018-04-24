//Ben Smedley
//bss170430@utdallas.edu
//Program 6 c++ source code



#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <locale>
#include <cstdint>
#include <sstream>
#include "cdk.h"

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

class BinaryFileHeader {

public:

  uint32_t magicNumber; //should be 0xFEEDFACE                                                           
  uint32_t versionNumber;
  uint64_t numRecords;

};

const int maxRecordStringLength = 25;
class BinaryFileRecord {

public:

  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];

};

template <class T>
string to_string(T t, ios_base & (*f)(ios_base&)) {

  ostringstream ss;
  ss << f << t;
  return ss.str();

}

int main() {

  cout << setprecision(10);
  ifstream binfile;

  BinaryFileHeader *header = new BinaryFileHeader();
  header->magicNumber = 0;
  header->versionNumber = 0;
  header->numRecords=0;

  BinaryFileRecord *record = new BinaryFileRecord();
  record->strLength = 0;


  WINDOW        *window;
  CDKSCREEN     *cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix                                                



  const char            *rowTitles[] = {"R0", "a", "b", "c", "d", "e"};
  const char            *columnTitles[] = {"C0", "a", "b", "c"};
  int           boxWidths[MATRIX_WIDTH+1] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int           boxTypes[MATRIX_WIDTH+1] = {vMIXED, vMIXED, vMIXED, vMIXED};

  /*                                                                                                     
   * Initialize the Cdk screen.                                                                          
   *                                                                                                     
   * Make sure the putty terminal is large enough                                                        
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*                                                                                                     
   * Create the matrix.  Need to manually cast (const char**) to (char **)                               
   */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths, boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  /*                                                                                                     
   * Dipslay a message                                                                                   
   */

  stringstream sstream;
  string temp;

  //first row - set to header of binary file                                                             
  binfile.open("cs3377.bin", ios::binary | ios::in);
  if(binfile.is_open()) {
    binfile.read(reinterpret_cast<char *>(&header->magicNumber), sizeof(header->magicNumber));

    sstream << "Magic: 0x" << std::hex << header->magicNumber;
    temp = sstream.str();

    setCDKMatrixCell(myMatrix, 1, 1, temp.c_str());
    sstream.clear();
    sstream.str("");
    binfile.read(reinterpret_cast<char *>(&header->versionNumber), sizeof(header->versionNumber));

    sstream << "Version: " << to_string(header->versionNumber);
    temp = sstream.str();
    setCDKMatrixCell(myMatrix, 1, 2, temp.c_str());

    sstream.clear();
    sstream.str("");
    binfile.read(reinterpret_cast<char *>(&header->numRecords), sizeof(header->numRecords));

    sstream << "NumRecords: " << header->numRecords;
    temp = sstream.str();
    setCDKMatrixCell(myMatrix, 1, 3, temp.c_str());

    for(int x = 0; x <header->numRecords && x < 4; x++) {

      sstream.str("");
      sstream.clear();
      if(!binfile.eof()) {

        binfile.read(reinterpret_cast<char *>(&record->strLength), sizeof(record->strLength));
        binfile.read(record->stringBuffer, sizeof(record->stringBuffer));

        sstream << "strlen: " << to_string(record->strLength);
        temp = sstream.str();
        setCDKMatrixCell(myMatrix, (x + 2), 1, temp.c_str());

        setCDKMatrixCell(myMatrix, (x + 2), 2, record->stringBuffer);

      }
    }

    drawCDKMatrix(myMatrix, true);    /* required  */
  }
  else
    cout << "Error! File did not open!" << endl;

  binfile.close();
  
  //Pause until a key is entered.
  string scratch;
  cin >> scratch ;
  // Cleanup screen                                                                                      
  endCDK();
}


