#include<cstdio>  
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<stack>
#include<string>
#include<iostream>
using namespace std;

//L1 Cache, block size 1 , single line cache 
#define Bytes_Per_Line 		32
#define CACHE_SIZE 			128
#define	BLOCK_SIZE 			4


//256 size
//128 data cache
///line of 1 
// #define iCACHE_WIDTH_BYTES_MAX	545		//largest blk & cache size gives tagsize= 8, offset=4, index=8. Cache has index+tag+32*blocksize+validbit


class iCache {
	public:
		int pendingMemory;

		int cacheSize;
		int blockSize;

		int offset;
		int	offsetSize;

		int index;
		int indexSize;

		int tag; 
		int tagSize;

		int PC;
		int data;
		int address[32];

		int cacheIndex[CACHE_SIZE];
		int cacheTag[CACHE_SIZE];
		bool cacheValid[CACHE_SIZE];
		int idata[((CACHE_SIZE/4)/BLOCK_SIZE)][BLOCK_SIZE]; 


		// int icache [iCACHE_HEIGHT_MAX][iCACHE_WIDTH];
		bool cacheHit;
		bool fillingCache;
		int numMisses;
		int numHits;
		
		iCache ();
		void parcePC();
		void icacheInit();
		void memorySetup();
		bool access();
		bool cacheWrite();
		int arrayToBinary(int left, int right);

};

iCache::iCache (){
	blockSize=BLOCK_SIZE;
	cacheSize= CACHE_SIZE; 

	offsetSize=0;
	indexSize=0;
	tagSize=0;
	pendingMemory=0;

	offset= 0b0;
	index= 0b0;
	tag= 0b0;
	data= 0b0;
	cacheHit= false;
	fillingCache= false;
	numMisses= 0;
	numHits= 0;

	for (int i=0; i<((CACHE_SIZE/4)/BLOCK_SIZE); i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			idata[i][j]=0;
		}
	}

	for (int i=0; i<CACHE_SIZE; i++){
		cacheIndex[i]=i;
		cacheTag[i]= -1; 	//init cache tag to invalid data
		cacheValid[i]= false;
	}


}

int iCache::arrayToBinary(int left, int right){
	int retVal=0b0;
	for(int i= left-1; i>=right; i--){
		retVal= (retVal<<1) | ((address[i])&1);
	}
	return retVal;	
}

void iCache::parcePC(){
//** initilize offset, index, and tag using address **//
//** setup cache size and fill index/valid bits **//

	cout<<BLOCK_SIZE<<endl; 
	offsetSize= log2(BLOCK_SIZE); 
	offset= arrayToBinary(offsetSize, 0);

	//setup index, which is size log(cache_size)
	indexSize= log2(CACHE_SIZE/4)-offsetSize;


	cout<<"Cache size= "<< CACHE_SIZE<< endl;
	cout<< "Index size= "<< indexSize << endl;
	cout<<"Offset Size= "<< offsetSize<<endl;
	index= arrayToBinary(indexSize+offsetSize, offsetSize);		//index will be from the offset size up till the index size
	cout<<"Index = "<< index <<endl;	
	cout<<"Offset= "<< offset <<endl; 

	//setup tag
	tagSize= 32-indexSize-2; 
	cout<< "Tag size= "<< tagSize<<endl;
	tag= arrayToBinary(tagSize+indexSize, indexSize);	//index size till end
	cout<<"Tag= "<< tag<<endl;
}
void iCache::icacheInit(){
	//setup memory from pipeline to here

}
bool iCache::access(){
	
	//Brandon math for miss clock cycles (6+2)*offsets
	pendingMemory= offset; 

	

	cacheTag[0]= 74565404;
	// cacheValid[0]= false;
	// idata[0][0]= 100;
	// idata[0][1]=22;
	// // idata[0][2]= 18;
	// idata[0][3]= 44;

	cout<< "Data= "<< idata[index][offset]<<endl;
	cout<<"valid= "<<cacheValid[index]<<endl;
	// for (int i=0; i<((CACHE_SIZE/4)/BLOCK_SIZE); i++){
	// 	for(int j=0; j<BLOCK_SIZE; j++){
	// 			cout<< "i= "<<i<<endl;
	// 			cout<<"j= "<<j<<endl;
	// 			cout<<"Begin maddness:"<<idata[i][j]<<endl;
	// 	}
	// }

	
	if(!fillingCache){
		if(cacheTag[index]==tag && cacheValid[index]==true){
			cout<<"Data should be 18"<<endl; 

			data= idata[index][offset];

			numHits++;
			cacheHit=true;
			return cacheHit;
		}
		else if((cacheTag[index]!=tag && cacheValid[index]==true) || (cacheTag[index]!=tag && cacheValid[index]==false)){
			//cache miss
			//access main memory

			// idata[index][offset]= memory[PC];
			cacheHit=false;
			numMisses++;
			return cacheHit;
		}
	}


	//Read data from cache using given address
	// if(fillingCache){
	// 	//wait for the cache to be done filling up
	// 	//Count down some clock cycles to wait ->counts as a cache miss
	// }
	//compare cache tag to tag of address to see if cache entry is there
	//if tag and valid bit are true, then read is a hit
}


// I want to check the index of PC with the index of my cache
	//So, index points to some block in the cache related to said index
	//That block is size[tag bit+valid]
	//That block data has size [blocksize * 32]
// Then I want to go into that block/line at that index and check my tag and valid
// If those are cool, then I want to grab the data
// It they ain't chillas, then I need to go to memory starting at ~00 of the index till the end of the ~11 of the index 
// essentially bit shift and take a shit when fill my block bitches