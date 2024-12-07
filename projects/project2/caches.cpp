#include<iostream>
#include<fstream>
#include<string>
#include<bitset> 
#include<array>
#include<stdlib.h>
#include<cmath>

using namespace std;

unsigned int get_address(unsigned int block_address, int target_bits, int offset_bits);
unsigned int get_block(unsigned int address, int target_bits, int offset_bits);
void cache_run(unsigned int *arr, int *marked_arr, unsigned int address, int *counter, int target_bits, int offset_bits);
void update_LRU(int *marked_arr, unsigned int index, int set_size, int pos);
int get_lru(int *marked_arr, unsigned int index, int set_size);
void set_assoc_cache_run(unsigned int *arr, int *marked_arr, unsigned int address, int *counter, int target_bits, int offset_bits, int set_size, int type);
int get_cold(int *marked_arr, unsigned int index, int set_size);
void update_hot_cold(int *marked_arr, unsigned int index, int set_size, int pos);
void two_level_cache_wt_run(unsigned int *arr1, int *marked_arr1, unsigned int *arr2, int *marked_arr2, unsigned int address, int *counter1, int *counter2, int *access_counter, int target_bits1, int offset_bits1, int set_size1, int target_bits2, int offset_bits2, int set_size2, string type);
void two_level_cache_wb_run(unsigned int *arr1, int *marked_arr1, unsigned int *arr2, int *marked_arr2, int *dirty, unsigned int address, int *counter1, int *counter2, int *access_counter, int target_bits1, int offset_bits1, int set_size1, int target_bits2, int offset_bits2, int set_size2, string type);
void two_level_cache_direct_wb_run(unsigned int *arr1, int *marked_arr1, unsigned int *arr2, int *marked_arr2, int *dirty, unsigned int address, int *counter1, int *counter2, int *access_counter, int target_bits1, int offset_bits1, int set_size1, int target_bits2, int offset_bits2, int set_size2, string type);


bool check_empty();


int main(int argc, char *argv[]) {
    ifstream infile(argv[1]);
    ofstream outfile(argv[2]);

    unsigned int addr;
    string type;

    int c = 0;

    //caches and vector for empty

    //caches for p1
    unsigned int cache1a[32];
    unsigned int cache1b[128];
    unsigned int cache1c[512];
    unsigned int cache1d[1024];
    int marked1a[32];
    int marked1b[128];
    int marked1c[512];
    int marked1d[1024];

    //caches for p2
    //16KB cache size, 32 byte cache blocks
    //2, 4, 8, 16 way sets => 256, 128, 64, 32 sets
    unsigned int cache2a[512];
    unsigned int cache2b[512];
    unsigned int cache2c[512];
    unsigned int cache2d[512];
    int marked2a[512] = {}; //512
    int marked2b[512] = {};
    int marked2c[512] = {};
    int marked2d[512] = {};

    //caches for p3
    //16KB cache size, 32 byte cache blocks, fully associative, i.e. 1 set
    unsigned int cache3a[512];
    int marked3a[512] = {};
    unsigned int cache3b[512];
    int marked3b[512] = {};

    //caches for p4
    //2way cache
    unsigned int cache4a[512];
    int marked4a[512] = {};
    unsigned int cache4b[1024];
    int marked4b[1024] = {};

    //caches for p5
    unsigned int cache5a[512];
    int marked5a[512] = {};
    unsigned int cache5b[1024];
    int marked5b[1024] = {};
    int dirty[512] = {};

    //caches for p6
    unsigned int cache6a[512];
    int marked6a[512] = {};
    unsigned int cache6b[1024];
    int marked6b[1024] = {};
    int dirtya[512] = {};
    unsigned int cache6c[512];
    int marked6c[512] = {};
    unsigned int cache6d[1024];
    int marked6d[1024] = {};
    int dirtyb[512] = {};

    int counters[18] = {};

    int access_counter[5] = {};

    while(infile >> std::hex >> type >> addr ) {

        access_counter[0] ++;

        if(c<100){
            c++;
        }

    
        //problem 1
        cache_run(cache1a, marked1a, addr, &(counters[0]), 5, 5); //
        cache_run(cache1b, marked1b, addr, &(counters[1]), 7, 5); //
        cache_run(cache1c, marked1c, addr, &(counters[2]), 9, 5); //
        cache_run(cache1d, marked1d, addr, &(counters[3]), 10, 5); //

        //problem 2
        set_assoc_cache_run(cache2a, marked2a, addr, &(counters[4]), 8, 5, 2, 0);
        set_assoc_cache_run(cache2b, marked2b, addr, &(counters[5]), 7, 5, 4, 0);
        set_assoc_cache_run(cache2c, marked2c, addr, &(counters[6]), 6, 5, 8, 0);
        set_assoc_cache_run(cache2d, marked2d, addr, &(counters[7]), 5, 5, 16, 0);

        //problem3
        set_assoc_cache_run(cache3a, marked3a, addr, &(counters[8]), 0, 5, 512, 0);
        set_assoc_cache_run(cache3b, marked3b, addr, &(counters[9]), 0, 5, 512, 1);


        //problem 4
        two_level_cache_wt_run(cache4a, marked4a, cache4b, marked4b, addr, &(counters[10]), &(counters[11]), &(access_counter[1]), 5, 5, 4, 7, 6, 8, type);

        //problem 5
        two_level_cache_wb_run(cache5a, marked5a, cache5b, marked5b, dirty, addr, &(counters[12]), &(counters[13]), &(access_counter[2]), 5, 5, 4, 7, 6, 8, type);

        //problem 6
        two_level_cache_wb_run(cache6a, marked6a, cache6b, marked6b, dirtya, addr, &(counters[14]), &(counters[15]), &(access_counter[3]), 5, 5, 4, 10, 6, 1, type);
        two_level_cache_direct_wb_run(cache6c, marked6c, cache6d, marked6d, dirtyb, addr, &(counters[16]), &(counters[17]), &(access_counter[4]), 5, 5, 4, 2, 6, 1024, type);

    }


    for(int i=0; i<4;i++){
        cout << counters[i] << "," << access_counter[0]  << "; ";
    }
    cout << endl;
    for(int i=0; i<4;i++){
        cout << counters[4+i] << "," << access_counter[0]  << "; ";
    }
    cout << endl;

    for(int i=0; i<2;i++){
        cout << counters[8+i] << "," << access_counter[0]  << "; ";
    }
    cout << endl;

    cout << counters[10] << "," << access_counter[0]  << "; ";
     cout << counters[11] << "," << access_counter[1]  << "; ";
     cout << endl;
     cout << counters[12] << "," << access_counter[0]  << "; ";
     cout << counters[13] << "," << access_counter[2]  << "; ";
     double k = 0;
    for(int i =0;i<1024;i++){
        if(cache5b[i] != 0){
            k++;
        }
    }
    cout << k/1024;
     cout << endl;
     cout << counters[14] << "," << access_counter[0]  << "; ";
     cout << counters[15] << "," << access_counter[2]  << "; ";
     k=0;
     for(int i =0;i<1024;i++){
        if(cache6b[i] != 0){
            k++;
        }
    }
    cout << std::setprecision(2) << k/1024;
     cout << endl;
     cout << counters[16] << "," << access_counter[0]  << "; ";
     cout << counters[17] << "," << access_counter[2]  << "; ";
     for(int i =0;i<1024;i++){
        if(cache6d[i] != 0){
            k++;
        }
    }
    cout << k/1024;
    cout << endl;

    



    infile.close();
    outfile.close();

    return 0;
}


//create get_block and implement that
unsigned int get_address(unsigned int block_address, int target_bits, int offset_bits){
    //unsigned int block_address = get_block(address, target_bits, offset_bits);
    if(target_bits == 0){
        return 0;
    }
    return block_address << (32-target_bits) >> (32-target_bits);
}

unsigned int get_block(unsigned int address, int target_bits, int offset_bits){
    return address >> offset_bits;
}

void cache_run(unsigned int *arr, int *marked_arr, unsigned int address, int *counter, int target_bits, int offset_bits){
    unsigned int block = get_block(address, target_bits, offset_bits);
    unsigned int index = get_address(block, target_bits, offset_bits);
    if(marked_arr[index] == 0){
        //cache currtently empty
        //cache miss
        marked_arr[index] = 1;
        arr[index] = block;
    } else {
        if(arr[index] != block){//cache miss
            arr[index] = block;
        } else {//cache hit
            (*counter) ++;
        }
    }
}

void set_assoc_cache_run(unsigned int *arr, int *marked_arr, unsigned int address, int *counter, int target_bits, int offset_bits, int set_size, int type){
    unsigned int block = get_block(address, target_bits, offset_bits);
    unsigned int index = get_address(block, target_bits, offset_bits);

    if(type == 0){
        if(marked_arr[index*set_size] == 0){
            //check if first one is empty
            //if empty
            //marked_arr[addr] = [0,0,0,0]
            //marked_arr[addr] = [2,1,3,4] -> [3,2,4,1]
            //ideally, it goes [1,0,0,0] -> [2,1,0,0]
            //only needs to be updated if

            update_LRU(marked_arr, index, set_size, 0);

            //update_hot_cold(marked_arr, index, set_size, 0);
            
            arr[index*set_size] = block;

        } else {
            //check if hit
            int i = 0;
            int hit = set_size;
            while(marked_arr[(index*set_size) + i] != 0 && i<set_size){
                if(arr[(set_size*index) + (i)] == block){
                    hit = i;
                }
                i++;
            }

            if(hit<set_size){
                //addpos somewhere
                (*counter)++;
                update_LRU(marked_arr, index, set_size, hit);
                //change LRU
            } else {
                //if not hit, then replace block
                int lru = get_lru(marked_arr, index, set_size);
                update_LRU(marked_arr, index, set_size, lru);

                arr[index*set_size + lru] = block;
            }
            //else, get position. Note, if space, that is position
            //for each non 0 in marked, also check which one
        }
    } else {
        int i = 0;
        int hit = set_size;
        while(i<set_size){
            if(arr[(set_size*index) + (i)] == block){
                hit = i;
            }
            i++;
        }

        if(hit<set_size){
            (*counter)++;
            update_hot_cold(marked_arr, index, set_size, hit);
        } else {
            int cold = get_cold(marked_arr, index, set_size);
            update_hot_cold(marked_arr, index, set_size, cold);
            arr[index*set_size + cold] = block;
        }
    }
    //get address
    //need to change effective address
    //get LRU from 0-set
    //insert arr[0 + set]

}

void two_level_cache_wt_run(unsigned int *arr1, int *marked_arr1, unsigned int *arr2, int *marked_arr2, unsigned int address, int *counter1, int *counter2, int *access_counter, int target_bits1, int offset_bits1, int set_size1, int target_bits2, int offset_bits2, int set_size2, string type){
    unsigned int block1 = get_block(address, target_bits1, offset_bits1);
    unsigned int index1 = get_address(block1, target_bits1, offset_bits1);
    unsigned int block2 = get_block(address, target_bits2, offset_bits2);
    unsigned int index2 = get_address(block2, target_bits2, offset_bits2);
    
    int i = 0;
    int hit = set_size1;
    while(marked_arr1[(index1*set_size1) + i] != 0 && i<set_size1){
        if(arr1[(set_size1*index1) + (i)] == block1){
            hit = i;
        }
        i++;
    }
    if(hit < set_size1){
        (*counter1)++;
        update_LRU(marked_arr1, index1, set_size1, hit);
    } else {
        int lru = get_lru(marked_arr1, index1, set_size1);
        update_LRU(marked_arr1, index1, set_size1, lru);
        arr1[index1*set_size1 + lru] = block1;
        //check L1
    }

    if(type == "S" || hit >= set_size1){
        (*access_counter) ++;
        int i = 0;
        int hit = set_size2;
        while(marked_arr2[(index2*set_size2) + i] != 0 && i<set_size2){
            if(arr2[(set_size2*index2) + (i)] == block2){
                hit = i;
            }
            i++;
        }

        if(hit < set_size2){
            (*counter2)++;
            update_LRU(marked_arr2, index2, set_size2, hit);
        } else {
            int lru = get_lru(marked_arr2, index2, set_size2);
            update_LRU(marked_arr2, index2, set_size2, lru);
            arr2[index2*set_size2 + lru] = block2;
        }
    }//for load
        //on hit in L1
            //update LRU
        //on miss in L1
            //replace in L1, update LRU
            //check L2
                //on hit in L2
                    //update LRU
                //on miss in L2
                    //replace in L1, update LRU
    //for store
        //on hit in L1
            //change LRU in L1
        //on miss in L1
            //replace in L1
                //search L2, change LRU and replace if necessary
}

void two_level_cache_wb_run(unsigned int *arr1, int *marked_arr1, unsigned int *arr2, int *marked_arr2, int *dirty, unsigned int address, int *counter1, int *counter2, int *access_counter, int target_bits1, int offset_bits1, int set_size1, int target_bits2, int offset_bits2, int set_size2, string type){
    unsigned int block1 = get_block(address, target_bits1, offset_bits1);
    unsigned int index1 = get_address(block1, target_bits1, offset_bits1);
    unsigned int block2 = get_block(address, target_bits2, offset_bits2);
    unsigned int index2 = get_address(block2, target_bits2, offset_bits2);

    int i = 0;
    int hit = set_size1;
    while(marked_arr1[(index1*set_size1) + i] != 0 && i<set_size1){
        if(arr1[(set_size1*index1) + (i)] == block1){
            hit = i;
        }
        i++;
    }
    if(hit < set_size1){
        (*counter1)++;
        update_LRU(marked_arr1, index1, set_size1, hit);
        if(type == "S"){
            dirty[index1*set_size1 + hit] = 1;
        }
    } else {
        int lru = get_lru(marked_arr1, index1, set_size1);
        if(type == "S"){
            dirty[index1*set_size1 + lru] = 1;
        }
        unsigned int block3 = arr1[index1*set_size1 + lru] >> 1;
        arr1[index1*set_size1 + lru] = block1;

        update_LRU(marked_arr1, index1, set_size1, lru);
        //get the dirty evicted block then the new block
        if(dirty[index1*set_size1 + lru] == 1){
            (*access_counter)++;
            dirty[index1*set_size1 + lru] = 0;
            unsigned int index3 = block3 & 127;
            
            int i = 0;
            int hit = set_size2;
            while(marked_arr2[(index3*set_size2) + i] != 0 && i<set_size2){
                if(arr2[(set_size2*index3) + (i)] == block3){
                    hit = i;
                }
                i++;
            }

            if(hit < set_size2){
                (*counter2)++;
                update_LRU(marked_arr2, index3, set_size2, hit);
            } else {
                int lru = get_lru(marked_arr2, index3, set_size2);
                update_LRU(marked_arr2, index3, set_size2, lru);
                arr2[index3*set_size2 + lru] = block3;
            }
            
        }
        int i = 0;
        int hit = set_size2;
        while(marked_arr2[(index2*set_size2) + i] != 0 && i<set_size2){
            if(arr2[(set_size2*index2) + (i)] == block2){
                hit = i;
            }
            i++;
        }

        if(hit < set_size2){
            (*counter2)++;
            update_LRU(marked_arr2, index2, set_size2, hit);
        } else {
            int lru = get_lru(marked_arr2, index2, set_size2);
            update_LRU(marked_arr2, index2, set_size2, lru);
            arr2[index2*set_size2 + lru] = block2;
        }
        (*access_counter)++;
        //check if dirty
    }
}


void two_level_cache_direct_wb_run(unsigned int *arr1, int *marked_arr1, unsigned int *arr2, int *marked_arr2, int *dirty, unsigned int address, int *counter1, int *counter2, int *access_counter, int target_bits1, int offset_bits1, int set_size1, int target_bits2, int offset_bits2, int set_size2, string type){
    unsigned int block1 = get_block(address, target_bits1, offset_bits1);
    unsigned int index1 = get_address(block1, target_bits1, offset_bits1);
    unsigned int block2 = get_block(address, 0, offset_bits2);
    unsigned int index2 = get_address(block2, 0, offset_bits2);

int i = 0;
    int hit = set_size1;
    while(marked_arr1[(index1*set_size1) + i] != 0 && i<set_size1){
        if(arr1[(set_size1*index1) + (i)] == block1){
            hit = i;
        }
        i++;
    }
    if(hit < set_size1){
        (*counter1)++;
        update_LRU(marked_arr1, index1, set_size1, hit);
        if(type == "S"){
            dirty[index1*set_size1 + hit] = 1;
        }
    } else {
        int lru = get_lru(marked_arr1, index1, set_size1);
        if(type == "S"){
            dirty[index1*set_size1 + lru] = 1;
        }
        unsigned int block3 = arr1[index1*set_size1 + lru] >> 1;
        arr1[index1*set_size1 + lru] = block1;

        update_LRU(marked_arr1, index1, set_size1, lru);
        //get the dirty evicted block then the new block
        if(dirty[index1*set_size1 + lru] == 1){
            (*access_counter)++;
            dirty[index1*set_size1 + lru] = 0;
            unsigned int index3 = 0;
            
            int i = 0;
            int hit = set_size2;
            while(marked_arr2[(index3*set_size2) + i] != 0 && i<set_size2){
                if(arr2[(set_size2*index3) + (i)] == block3){
                    hit = i;
                }
                i++;
            }

            if(hit < set_size2){
                (*counter2)++;
                update_LRU(marked_arr2, index3, set_size2, hit);
            } else {
                int lru = get_lru(marked_arr2, index3, set_size2);
                update_LRU(marked_arr2, index3, set_size2, lru);
                arr2[index3*set_size2 + lru] = block3;
            }
            
        }
        int i = 0;
        int hit = set_size2;
        while(marked_arr2[(index2*set_size2) + i] != 0 && i<set_size2){
            if(arr2[(set_size2*index2) + (i)] == block2){
                hit = i;
            }
            i++;
        }

        if(hit < set_size2){
            (*counter2)++;
            update_LRU(marked_arr2, index2, set_size2, hit);
        } else {
            int lru = get_lru(marked_arr2, index2, set_size2);
            update_LRU(marked_arr2, index2, set_size2, lru);
            arr2[index2*set_size2 + lru] = block2;
        }
        (*access_counter)++;
        //check if dirty
    }
}

//only ever called if not empty
int get_lru(int *marked_arr, unsigned int index, int set_size){
    //problem here
    int least = 0;
    int pos = 0;
    for(int i=0; i<set_size; i++){
        if(marked_arr[index*set_size + i]>least){
            least = marked_arr[index*set_size + i];
            pos = i;
        } else if (marked_arr[index*set_size + i] == 0){
            pos = i;
            break;
        }
    }
    return pos;
}

//if hit, makes a 0
void update_LRU(int *marked_arr, unsigned int index, int set_size, int pos){
    int is_zero = set_size;
    for(int i=0; i<set_size; i++){
        if(marked_arr[index*set_size + i] == 0){
            is_zero = i;
            break;
        }
    }

    if(is_zero<set_size){
        //if there is a 0
        for(int i=0; i < is_zero; i++){
            marked_arr[index*set_size + i]++;
        }
        marked_arr[index*set_size + pos] = 1;
    } else {
        for(int i=0; i<set_size; i++){
            if(marked_arr[index*set_size + i] < marked_arr[index*set_size + pos]){
                marked_arr[index*set_size + i]++;
            }
        }
        marked_arr[index*set_size + pos] = 1;
    }
}

void update_hot_cold(int *marked_arr, unsigned int index, int set_size, int pos){
    int levels = static_cast<int>(log(set_size) / log(2));
    int tree_pos = 0;
    int curr = pos;
    for(int i=0; i<levels;i++){
        if(curr>(set_size/(static_cast<int>(pow(2, i+1))))-1){ //chck this
            marked_arr[index*set_size + tree_pos]=1;
            curr -= (set_size/(static_cast<int>(pow(2, i+1))));
            tree_pos = 2*tree_pos + 2;
        } else {
            marked_arr[index*set_size + tree_pos]=0;
            tree_pos = 2*tree_pos + 1;
        }
        //cout << index*set_size + tree_pos << endl;
    }
}

int get_cold(int *marked_arr, unsigned int index, int set_size){
    int levels = static_cast<int>(log(set_size) / log(2));
    int pos = 0;
    int tree_pos = 0;
    for(int i=0; i<levels;i++){
        if(marked_arr[index*set_size + tree_pos]==1){
            tree_pos = 2*tree_pos + 1;
        } else {
            pos += set_size/(static_cast<int>(pow(2, i+1)));
            tree_pos = 2*tree_pos + 2;
        }
    }
    return pos;
}


//mask holds int
//


//get address relevant for all
//32 bit size, 32 bit offset. Thus, 5 bit offset

//direct-mapped cache
//1, 4, 16, 32 KB
//figure out how large
//only use vector? and replace?

//Set-associative cache
//16KB, 2, 4, 8, 16
//
//implement LRU with Leasrt recently used with queue, queue for each set
//hot and cold LRU for each set

//Fully-associative cache
//LRU and cache

//2 level cache with write through
//LRU

//2 level cache with write 

