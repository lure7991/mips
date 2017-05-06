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

		int hitRate;
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

		int icacheIndex[CACHE_SIZE];
		int icacheTag[CACHE_SIZE];
		bool icacheValid[CACHE_SIZE];
		int idata[((CACHE_SIZE/4)/BLOCK_SIZE)][BLOCK_SIZE]; 


		bool icacheHit;
		int numMisses;
		int numHits;
		
		iCache ();
		void parcePC_icache();
		void icacheInit();
		void memorySetup();
		bool access();
		int arrayToBinary(int left, int right);

};

iCache::iCache (){
	blockSize=BLOCK_SIZE;
	cacheSize= CACHE_SIZE; 

	offsetSize=0;
	indexSize=0;
	tagSize=0;

	offset= 0b0;
	index= 0b0;
	tag= 0b0;
	data= 0b0;
	icacheHit= false;
	numMisses= 0;
	numHits= 0;

	for (int i=0; i<((CACHE_SIZE/4)/BLOCK_SIZE); i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			idata[i][j]=0;
		}
	}

	for (int i=0; i<CACHE_SIZE; i++){
		icacheIndex[i]=i;
		icacheTag[i]= -1; 	//init cache tag to invalid data
		icacheValid[i]= false;
	}


}

int iCache::arrayToBinary(int left, int right){
	int retVal=0b0;
	for(int i= left-1; i>=right; i--){
		retVal= (retVal<<1) | ((address[i])&1);
	}
	return retVal;	
}

void iCache::parcePC_icache(){
//** initilize offset, index, and tag using address **//
//** setup cache size and fill index/valid bits **//

	cout<<BLOCK_SIZE<<endl; 
	offsetSize= log2(BLOCK_SIZE); 
	offset= arrayToBinary(offsetSize, 0);
	indexSize= log2(CACHE_SIZE/4)-offsetSize;

	cout<<"Cache size= "<< CACHE_SIZE<< endl;
	cout<< "Index size= "<< indexSize << endl;
	cout<<"Offset Size= "<< offsetSize<<endl;

	index= arrayToBinary(indexSize+offsetSize, offsetSize);		//index will be from the offset size up till the index size
	
	cout<<"Index = "<< index <<endl;	
	cout<<"Offset= "<< offset <<endl; 

	//setup tag
	tagSize= 32-indexSize-2; 
	tag= arrayToBinary(tagSize+indexSize, indexSize);	//index size till end
	
	cout<< "Tag size= "<< tagSize<<endl;
	cout<<"Tag= "<< tag<<endl;
}

bool iCache::access(){
	
	if(icacheTag[index]==tag && icacheValid[index]==true){
		data= idata[index][offset];
		numHits++;
		icacheHit=true;
		return icacheHit;
	}
	else if((icacheTag[index]!=tag && icacheValid[index]==true) || (icacheTag[index]!=tag && icacheValid[index]==false)){
		icacheTag[index]= tag;
		icacheValid[index]= true;
		icacheHit=false;
		numMisses++;
		return icacheHit;
	}
	hitRate= numHits/(numHits+numMisses);
	return hitRate;
	//hit, set dirty bit to 1
	//miss , check policy if WB check dirty, if 1 update mem with cache before reload 
	//dirty bit zero, cache and mem the same
	//update contents, set to 0
	//hits always make it 1
	//compare cache tag to tag of address to see if cache entry is there
	//if tag and valid bit are true, then read is a hit
}
