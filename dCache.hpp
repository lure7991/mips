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

class dCache {
public:	
	bool write;
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

	int dcacheIndex[CACHE_SIZE];
	int dcacheTag[CACHE_SIZE];
	bool dcacheValid[CACHE_SIZE];
	bool dirtyBit[CACHE_SIZE];
	int dData[((CACHE_SIZE/4)/BLOCK_SIZE)][BLOCK_SIZE]; 


	// int icache [iCACHE_HEIGHT_MAX][iCACHE_WIDTH];
	bool dcacheHit;
	bool fillingCache;
	int numMisses;
	int numHits;
	
	dCache ();
	void parcePC_dcache();
	void dcacheInit();
	void memorySetup();
	bool access();
	bool cacheWrite();
	int arrayToBinary(int left, int right);
};

dCache::dCache (){
	write= 0; 

	blockSize=BLOCK_SIZE;
	cacheSize= CACHE_SIZE; 

	offsetSize=0;
	indexSize=0;
	tagSize=0;

	offset= 0b0;
	index= 0b0;
	tag= 0b0;
	data= 0b0;
	dcacheHit= false;
	fillingCache= false;
	numMisses= 0;
	numHits= 0;

	for (int i=0; i<((CACHE_SIZE/4)/BLOCK_SIZE); i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			dData[i][j]=0;
		}
	}

	for (int i=0; i<CACHE_SIZE; i++){
		dcacheIndex[i]=i;
		dcacheTag[i]= -1; 	//init cache tag to invalid data
		dcacheValid[i]= false;
	}
}
int dCache::arrayToBinary(int left, int right){
	int retVal=0b0;
	for(int i= left-1; i>=right; i--){
		retVal= (retVal<<1) | ((address[i])&1);
	}
	return retVal;	
}

void dCache::parcePC_dcache(){
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

bool dCache::access(){
	
	//Brandon math for miss clock cycles (6+2)*offsets


	cout<< "Data= "<< dData[index][offset]<<endl;
	cout<<"valid= "<<dcacheValid[index]<<endl;

	if(dcacheTag[index]==tag && dcacheValid[index]==true){
		data= dData[index][offset];
		numHits++;
		dcacheHit=true;
		return dcacheHit;
	}
	else if((dcacheTag[index]!=tag && dcacheValid[index]==true) || (dcacheTag[index]!=tag && dcacheValid[index]==false)){
			dcacheTag[index]= tag;
			dcacheValid[index]= true;
			dcacheHit=false;
			numMisses++;
			return dcacheHit;

		}
	// else if((dcacheTag[index]!=tag && dcacheValid[index]==true) || (dcacheTag[index]!=tag && dcacheValid[index]==false)){
	// 	dcacheTag[index]= tag;
	// 	dcacheValid[index]= true;
	// 	dcacheHit=false;
	// 	numMisses++;
	// 	return dcacheHit;
	// }
		hitRate= numHits/(numHits+numMisses);
	return hitRate;
	}