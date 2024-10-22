/* Example
 * Parsing Branch Traces
 */

#include<iostream>
#include<fstream>
#include<string>
#include<bitset> 
#include<array>
#include<stdlib.h>

using namespace std;

void run_prediction(unsigned long long addr, string behavior, int* counts, int (&bimodal_tables)[4][7][4096], int (&b_sizes)[7], int (&gshare_tables)[12][4096], int (&gshare_values)[12], int (&selector_table)[4096]);
void print_predictions(int* counts, int count, ofstream& outfile);
bool bimodal_predict(unsigned long long addr, int count_size, int rel_index, int table_size, int size_incrementer,int taken, int (&bimodal_tables)[4][7][4096]);
bool gshare_predict(unsigned long long addr, int gshare_size, int rel_index, int taken, int (&gshare_tables)[12][4096], int (&gshare_values)[12]);
bool tournament_predict(unsigned long long addr, int taken, int (&selector_table)[4096], int (&bimodal_tables)[4][7][4096], int (&gshare_tables)[12][4096], int (&gshare_values)[12]);
int new_counter_value(int counter, int count_size, int result);

int main(int argc, char *argv[]) {
  //b_sizes represent the 7 different sizes of bits we use to index the bimodal tables
  int b_sizes[7] = {2, 3, 5, 6, 8, 10, 12};

  //counts keeps tracks of how many branches are predicted
  int* counts = new int[35]();
  //0 for part 1
  //1 for part 2...
  //2-8 for 3
  //9-15 for 4
  //16-22 for 5
  //23-33 for 6
  //34 for 7

  //count keeps track of total braches
  int count = 0;

  //this creates the bimodal tables we use, with all tables set to 0 by default
  int bimodal_tables[4][7][4096] = { 0 };

  //the 2nd bimodal tables are set to 7
  for(int i=0;i<7;i++){
    for(int k=0;k<4096;k++){
      bimodal_tables[2][i][k] = 7;
    }
  }

  //initialized GHR values for gshare table at 1
  int gshare_values[12] = {0};
  for(int i=0;i<11;i++){
    gshare_values[i] = 1;
    //cout << std::bitset<12>(gshare_values[i]) << endl;
  }
  //gshare_values[12] = 0 for problem 7
  int gshare_tables[12][4096] = { 0 };

  //for the tournament selector table
  int selector_table[4096] = { 0 };
  for(int i=0;i<4096;i++){
    selector_table[i] = 3;
  }

  // Temporary variables
  unsigned long long addr;
  string behavior;

  // Open file for reading
  ifstream infile(argv[1]);
  ofstream outfile(argv[2]);

  // c is used for testing a small number of lines from a file
  int c = 0;
  while(infile >> std::hex >> addr >> behavior) {

    if(c<20){
      run_prediction(addr, behavior, counts, bimodal_tables, b_sizes, gshare_tables, gshare_values, selector_table);
      //c++;
    }
    count ++;
  }

  print_predictions(counts, count, outfile);

  delete[] counts;

  infile.close();
  outfile.close();
  return 0;
}

void run_prediction(unsigned long long addr, string behavior, int* counts, int (&bimodal_tables)[4][7][4096], int (&b_sizes)[7], int (&gshare_tables)[12][4096], int (&gshare_values)[12], int (&selector_table)[4096]){
    int taken = 1;
    if(behavior == "NT"){
      taken = 0;
    }

  //Problems 1 and 2
    if(taken == 1){
      counts[0]++;
    } else {
      counts[1]++;
    }

  //Problems 3,4,5
  for(int i=1;i<4;i++){
    for(int k=0;k<7;k++){
      //bimodal_predict uses a read in the address and result of an instruction, and uses the size of the counter and size of the table to predict the instruction, and the success of the prediction returned and the table updated accordingly
      if(bimodal_predict(addr, i, i-1, b_sizes[k], k, taken, bimodal_tables)){
        counts[2 + ((i-1)*7) + k]++;
      }
    }
  }

  //Problem 6
  for(int i=2;i<13;i++){
    if(gshare_predict(addr, i, i-2, taken, gshare_tables, gshare_values)){
        counts[23+(i-2)]++;
    }
  }

  //Problem 7
  if(tournament_predict(addr, taken, selector_table, bimodal_tables, gshare_tables, gshare_values)){
    counts[34]++;
  }

}

bool tournament_predict(unsigned long long addr, int taken, int (&selector_table)[4096], int (&bimodal_tables)[4][7][4096], int (&gshare_tables)[12][4096], int (&gshare_values)[12]){
  int index;
  int prediction;
  int prediction_cleaned;
  bool bimodal;
  bool gshare;
  int index_sizer = (1 << 12) - 1; //12 bit index
  index = addr & index_sizer;

  prediction = (selector_table)[index];
  prediction_cleaned = (prediction >> 1) & 1;

  bimodal = bimodal_predict(addr, 3, 3, 12, 0, taken, bimodal_tables);
  //bimodal with 3 state, index to 3rd table, 12 bit index, which array of bimodal_tables[3][4096] we use
  gshare = gshare_predict(addr, 12, 11, taken, gshare_tables, gshare_values);
  //gshare with 12 bits, index to 11th gshare_tables

  if(prediction_cleaned == 1){
    //we predict 1, bimodal
    if(bimodal != gshare){
      if(bimodal){
        (selector_table)[index] = new_counter_value(prediction, 2, 1);//shift towards 11, bimodal
        return true;
      } else {
        (selector_table)[index] = new_counter_value(prediction, 2, 0);//shift towards 00, gshare
        return false;
      }
    } 
    return bimodal; 
    //if both were true, do nothing
  } else {
    if(bimodal != gshare){
      if(gshare){
        (selector_table)[index] = new_counter_value(prediction, 2, 0);//shift towards 00, gshare
        return true;
      } else {
        (selector_table)[index] = new_counter_value(prediction, 2, 1);//shift towards 11, bimodal
        return false;
      }
    } 
    return gshare; 
  }

  //given selector table, select one
  //gshare_predict(addr, 12, 11, taken, gshare_tables, gshare_values);
}


bool gshare_predict(unsigned long long addr, int gshare_size, int rel_index, int taken, int (&gshare_tables)[12][4096], int (&gshare_values)[12]){
  //we implement gshare_predict such that each index is 12 bits long, and we keep index_size bits of gshare
  int index;
  int prediction;
  int prediction_cleaned;
  //position within gshare_tables and gshare_values is index-2; indexes 2-12, table index 0-10
  int temp_gshare_value = gshare_values[rel_index];
  int gshare_sizer = (1 << gshare_size) - 1;
  //given gshare_size n, creates a mask of n rightmost 1 bits. For example, gshare_size 3, 2 << 3 - 1 = 7 = 111, 3 rightmost 1 bits
  int index_sizer = (1 << 12) - 1;
  //our index is always 12 bits
  
  index = (addr & index_sizer)^(temp_gshare_value);
  //XOR gshare with last 12 bits of PC

  prediction = (gshare_tables)[rel_index][index];
  //prediction is always in the form x1x2, our prediction is at x1

  prediction_cleaned = (prediction >> 1) & 1;

  //same saturating bit scheme implemented with bimodal predictor with 2 bits
  (gshare_tables)[rel_index][index] = new_counter_value(prediction, 2, taken);

  //to update gshare value, shift to the left and add rightmost bit depending on result and maintain 
  temp_gshare_value = ((temp_gshare_value << 1) | taken) & gshare_sizer;

  gshare_values[rel_index] = temp_gshare_value;
 
  return taken == prediction_cleaned;
}


bool bimodal_predict(unsigned long long addr, int count_size, int rel_index, int table_size, int size_incrementer, int taken, int (&bimodal_tables)[4][7][4096]){
  int index;
  int prediction;
  int prediction_cleaned;
  index = addr & ((1 << table_size) - 1);

  //get our raw prediction
  prediction = (bimodal_tables)[rel_index][size_incrementer][index];
  //cout << std::bitset<4>(prediction) << " shift " << count_size-1 << "->" ;

  //interpret our prediction. Our predictions are set up such that the left most significant bit determines the outome. We shift this bit to the right, so all cleaned predictions are either 0 or 1
  prediction_cleaned = prediction >> (count_size-1);
  //cout << std::bitset<4>(prediction) << "->" ;
  prediction_cleaned = prediction_cleaned & 1;
  //cout << std::bitset<4>(prediction) << endl;

  //get the new value for the table based on the result. Different schemes are implemented in new_counter_value values
  (bimodal_tables)[rel_index][size_incrementer][index] = new_counter_value(prediction, count_size, taken);
  
  return taken == prediction_cleaned; 
}

int new_counter_value(int counter, int count_size, int result){
  if(count_size==1){
    //for our 1 bit bimodal prediction, the next prediction is the result.
    return result;
  } else if (count_size==2){
    //for our 2 bit bimodal prediction, we used saturating counters, which requires 2 consecutive not taken values for a strongly taken value to choose not take
    //either 11, 10, 01, 00
    //if 1 -> 11, 11, 10, 01
    //if 0 -> 10, 01, 00, 00
    if(result == 1){
      if(counter < 3){
        counter ++;
      }
    } else {
      if(counter > 0){
        counter--;
      }
    }
    return counter;
  } else {
    //for our 3 bit bimodal prediction, we use an arbitrary 6 state diagram given in the assignment sheet, described as follows
    //either (1)111(7), (2)110(6), (3)100(4), (4)011(3), (5)001(1), (6)000(0)
    //if 0 -> 2(110)(6), (4)011(3), (4)011(3), (5)001(1), (6)000(0), (6)000(0)
    //if 1 -> (1)111(7), (1)111(7), (2)110(6), (3)100(4), (4)011(3), (5)001(1)
    if(result == 1){
      switch(counter){
        case 0:
          return 1;
          break;
        case 1:
          return 3;
          break;
        case 3:
          return 4;
          break;
        case 4:
          return 6;
          break;
        case 6:
          return 7;
          break;
        case 7:
          return 7;
          break;
      }
    } else {
      switch(counter){
        case 0:
          return 0;
          break;
        case 1:
          return 0;
          break;
        case 3:
          return 1;
          break;
        case 4:
          return 3;
          break;
        case 6:
          return 3;
          break;
        case 7:
          return 6;
          break;
      }
    }
  }
  return 0;
}

void print_predictions(int* counts, int count, ofstream& outfile){
  //1 and 2
  outfile << counts[0] << "," << count << ";" << endl;
  outfile << counts[1] << "," << count << ";" << endl;

  //3, 4, 5
  for(int k=2;k<23;k=k+7){
    for(int i=k;i<k+6;i++){
      outfile << counts[i] << "," << count << ";" << " ";
    }
    outfile << counts[k+6] << "," << count << ";" << endl;
  }

  //6
  for(int i=23;i<33;i++){
    outfile << counts[i] << "," << count << ";" << " ";
  }
  outfile << counts[33] << "," << count << ";" << endl;

  //7
  outfile << counts[34] << "," << count << ";" << endl;
}

//for the tables
//for 3, 4, and 5: 7 arrays of - [2], [4], [8], [32], [64], [256], [1024], [4096] lengths
//for 6: 11 arrays of lengths [2^2] s.t. 2->12
//for 7: 3 arrays of length [4096]

// 1 - always taken
// 2 - never taken
// 3 - bimodal w 1 bit for table sizes of 4(2), 8(3), 32(5), 64(6), 256(8), 1024(10), 4096(12)
// 4 - bimodal w 2
// 5 - bimodal w 3 s.t. 
// 6 - gshare
// 7 - tournament 

