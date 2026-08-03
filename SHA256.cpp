#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<cstdint>
#include<iomanip>
#include<sstream>
using namespace std;

void app(vector<int>& inital, const vector<int>& final){
    inital.insert(inital.end(),final.begin(),final.end());
}

uint32_t rotateRight(uint32_t x, int n){
    return (x>>n)|(x<<(32-n));
}
uint32_t smallSigma0(uint32_t x){
    return rotateRight(x, 7)^rotateRight(x, 18)^(x >> 3);
}
uint32_t smallSigma1(uint32_t x) {
    return rotateRight(x,17)^rotateRight(x,19)^(x >>10);
}
uint32_t bigSigma0(uint32_t x){
    return rotateRight(x, 2)^ rotateRight(x, 13)^ rotateRight(x, 22);
}
uint32_t bigSigma1(uint32_t x){
    return rotateRight(x, 6)^ rotateRight(x, 11)^ rotateRight(x, 25);
}
uint32_t choice(uint32_t x, uint32_t y, uint32_t z){
    return (x & y) ^ (~x & z);
}

uint32_t majority(uint32_t x, uint32_t y, uint32_t z){
    return (x & y) ^ (x & z) ^ (y & z);
}
uint32_t h0 = 0x6a09e667;
uint32_t h1 = 0xbb67ae85;
uint32_t h2 = 0x3c6ef372;
uint32_t h3 = 0xa54ff53a;
uint32_t h4 = 0x510e527f;
uint32_t h5 = 0x9b05688c;
uint32_t h6 = 0x1f83d9ab;
uint32_t h7 = 0x5be0cd19;
const uint32_t k[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};



vector<int> toBinary(char c){
    int num=static_cast<int>(c);
    vector<int> binary(0);
    int pow=128;
    while(pow>=1){
        if(num-pow<0){
            binary.push_back(0);
            pow/=2;
        }
        else{
            binary.push_back(1);
            num=num-pow;
            pow/=2;
        }
    }
    return binary;
}

vector<int> toBinary(string buffer){
    vector<int> binary(0);
    for(int i=0;i<buffer.length();i++){
        app(binary,toBinary(buffer[i]));
    }
    return binary;
}

vector<int> convertFileToBinary(const string& name){
    vector<int> binaryCode;
    ifstream in(name, ios::binary);
    char c;
    while (in.get(c)) {
        uint8_t byte = static_cast<uint8_t>(static_cast<unsigned char>(c));
        for (int bit = 7; bit >= 0; --bit) {
            binaryCode.push_back((byte >> bit) & 1U);
        }
    }
    return binaryCode;
}

void padding(vector<int>& binaryCode) {
    uint64_t originalBitLength = binaryCode.size();
    binaryCode.push_back(1);

    while (binaryCode.size() % 512 != 448) {
        binaryCode.push_back(0);
    }
    for (int bit = 63; bit >= 0; --bit) {
        binaryCode.push_back(
            (originalBitLength >> bit) & 1ULL
        );
    }
}

vector<vector<int>> split512Chunks(const vector<int>&paddedBinaryCode){
    vector<vector<int>> split512Chunks(0);
  
    for(long i=0;i<paddedBinaryCode.size();i+=512){
        vector<int> chunk;
        for(int j=0;j<512;j++){
            chunk.push_back(paddedBinaryCode[i+j]);

        }
        split512Chunks.push_back(chunk);
    }
    return split512Chunks;
}

vector<uint32_t>split512ChunkTo32Bits(const vector<int>& chunk){
    vector<uint32_t> w(0);
    uint32_t value;
    for(int i=0;i<chunk.size();i+=32){
        value = 0;
        for (int j = 0; j < 32; j++) {
            value <<= 1;        // Shift left by one bit
            value |= chunk[i+j];   // Add the next bit (0 or 1)
        }
        w.push_back(value);
    }
    //done with first 16 chunks
    int i=16;
    while(w.size()!=64){
        uint32_t num=w[i-16]+ smallSigma0(w[i-15])+ w[i-7]+ smallSigma1(w[i-2]);
        w.push_back(num);
        i++;
    }
    return w;
}

void mixup(const vector<uint32_t>& w){
    uint32_t a = h0;
    uint32_t b = h1;
    uint32_t c = h2;
    uint32_t d = h3;
    uint32_t e = h4;
    uint32_t f = h5;
    uint32_t g = h6;
    uint32_t h = h7;
    for (int i = 0; i < 64; i++){
        uint32_t temp1 =h+ bigSigma1(e)+ choice(e, f, g)+ k[i]+ w[i];
        uint32_t temp2 =bigSigma0(a)+ majority(a, b, c);
        h=g;
        g=f;
        f=e;
        e=d + temp1;
        d=c;
        c=b;
        b=a;
        a = temp1 + temp2;
    }
    h0 += a;
    h1 += b;
    h2 += c;
    h3 += d;
    h4 += e;
    h5 += f;
    h6 += g;
    h7 += h;
    
    
    

}
void resetHashVals(){
    h0 = 0x6a09e667;
    h1 = 0xbb67ae85;
    h2 = 0x3c6ef372;
    h3 = 0xa54ff53a;
    h4 = 0x510e527f;
    h5 = 0x9b05688c;
    h6 = 0x1f83d9ab;
    h7 = 0x5be0cd19;
}
string finalHash() {
    resetHashVals();
    stringstream output;

    output<<hex<<setfill('0')<<setw(8)<<h0<<setw(8)<<h1<<setw(8)<<h2<<setw(8)<<h3<<setw(8)<<h4<< setw(8) << h5<< setw(8) << h6<< setw(8) << h7;

    return output.str();

}
string hashFile(string fileName){
    vector<int> text =convertFileToBinary(fileName);
    padding(text);
    vector<vector<int>> blocks =split512Chunks(text);
    for (const vector<int>& block : blocks) {
        vector<uint32_t> w =split512ChunkTo32Bits(block);
        mixup(w);
    }
    return finalHash();
}



