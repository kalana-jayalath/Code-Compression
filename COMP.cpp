#include<iostream>
#include<unordered_map>
#include<fstream>
#include<vector>
#include<algorithm>
#include<map>
#include<bitset>
#include<string.h>

using namespace std;
string DM       = "101";
string ORIGINAL = "110";
string RLE      = "000";



vector <string> uncompressed_bits ;
vector <string> compressed_bits;

map <int , string> Dictionary;



struct frequency_bitval{
        int fr;
        string bit;
};

bool sortcol( frequency_bitval v1, frequency_bitval v2 ) { 
    return (v1.fr < v2.fr); 
} 

string to_binary(int n){
    string r;
        while(n!=0) {
            r=(n%2==0 ?"0":"1")+r; 
            n/=2;
        }
        return r;
}


/*========================= Direct matching ==============================*/
int Direct_matching( vector<string> bitvals , map<int , string> x = Dictionary ){
    string compressed;
    for(int j(0) ; j< size(bitvals) ; j++){
        
        cout << j << "****" <<endl;
        cout << bitvals.at(j) << endl;
        
        for(int i(0) ; i<8 ; i++){
            if( Dictionary[i] == bitvals.at(j)){
                string binary = bitset<3>(i).to_string();
                compressed = (DM+binary); 
                cout << compressed<< endl;
                cout << "match" << endl;
                break;
                
            }
            else{
                compressed = ORIGINAL + bitvals.at(j);
                cout << compressed<< endl;
                break;
                
            }
        }
    }
    
    return(0);
}

/*===================Run length encoding =============================*/
int Run_Length_Encoding( vector<string> bitvals , map<int , string> x = Dictionary ){
    string compressed;
    
    string rle;
    int step(0);
    for(int j(0) ; j< size(bitvals) ; j++){
        
        cout << j << "****" <<endl;
        cout << bitvals.at(j) << endl;
        
        for(int i(0) ; i<8 ; i++){
            
            if( Dictionary[i] == bitvals.at(j)){
                string rle_compressed;
                string binary = bitset<3>(i).to_string();
                compressed = (DM+binary); 
                cout << compressed<< endl;
                cout << "match" << endl;

                
                if(bitvals.at(j) == bitvals.at(j+1)){
                    string r0 = "01";
                    rle_compressed = RLE+r0;
                    
                    cout << r0<< endl;
                    cout << rle_compressed<< "rle" <<endl;
                    step = step +2;

                    if(Dictionary[i] == bitvals.at(j+2)){
                        string r1 = "01";
                        rle_compressed = (RLE+r1);
                        step = step +1;
                        cout << r1<< endl;
                        cout << rle_compressed<< "rle" <<endl;

                        if(Dictionary[i] == bitvals.at(j+3)){
                            string r2 = "10";
                            rle_compressed = (RLE+r2);
                            step = step +1;
                            cout << r2<< endl;
                            cout << rle_compressed<< "rle" <<endl;
                            
                            if(Dictionary[i] == bitvals.at(j+4)){
                                string r3 = "11";
                                rle_compressed = (RLE+r3);
                                step = step +1;
                                cout << r3<< endl;
                                cout << rle_compressed<< "rle" <<endl;
                                break;
                            }
                            
                            
                        }
                        
                    }
                    
                    
                }
                
                break;
            }
            else{
                compressed = ORIGINAL + bitvals.at(j);
                cout << compressed<< endl;
                break;
                
            }
        }
    }
    
    return(0);
}
/*===========================decompression=============================================*/
vector <string> Decompresssed_bitpat;
int decompression (vector <string> long_bit_line ,vector <string> dictionary_index , vector <string> dictionary){
    int step(0);
    string cmp_method;
    string bit_pat ;
    int index   ; 
    string ref_pattern;
    string rle_count ; 
    for(int i(0) ; i < long_bit_line.size() ; +step){
        /*original*/
        if(long_bit_line.at(0).substr(i,3) == "110"){
            bit_pat = long_bit_line.at(0).substr(i+3,32);
            Decompresssed_bitpat.push_back(bit_pat);
            step = 35;
        }
        /*direct matching*/
        else if(long_bit_line.at(0).substr(i,3) == "101"){
            ref_pattern = long_bit_line.at(0).substr(i+3,3); /*slicing index bits*/
            for (int i(0) ; i < 8 ; i++){
                if(dictionary_index.at(i) == ref_pattern){
                    index = i;
                }
            }
            bit_pat = dictionary.at(index);
            Decompresssed_bitpat.push_back(bit_pat);
            step = 6;
        }
        /*RLE*/
        else if(long_bit_line.at(0).substr(i,3) == "000"){
            bit_pat   = Decompresssed_bitpat.back();
            rle_count = long_bit_line.at(0).substr(i+3,2);

            if(rle_count == "00"){
                Decompresssed_bitpat.push_back(bit_pat);    
            }
            else if(rle_count == "01"){
                Decompresssed_bitpat.push_back(bit_pat);
                Decompresssed_bitpat.push_back(bit_pat);    
            }
            else if(rle_count == "10"){
                Decompresssed_bitpat.push_back(bit_pat);
                Decompresssed_bitpat.push_back(bit_pat);
                Decompresssed_bitpat.push_back(bit_pat);    
            }
            else if(rle_count == "11"){
                Decompresssed_bitpat.push_back(bit_pat);
                Decompresssed_bitpat.push_back(bit_pat);
                Decompresssed_bitpat.push_back(bit_pat);
                Decompresssed_bitpat.push_back(bit_pat);    
            }
            step = 5;
        }
    }
}



int main(){
    
   
    /*========reading all the uncompressed bit lines from an input text file and storing them in a vector========= */
    ifstream input_file {"original.txt"};
    string line{};

    if(!input_file){
        cerr << "File was not opened correctly. error" << endl;
        return 1;
    }
    
    int i (0);
    while (!input_file.eof()){
        getline(input_file , line);
        uncompressed_bits.push_back (line); 
        i++;

    }
    input_file.close();

    

   /*===========iterating through the vector and storing frequency of each unique bit pattern with pattern it self ============*/
    int frequency(0);
    int bit_count (size(uncompressed_bits));
    vector <int> checked_uncompressed_bits(bit_count, 0);
    
    vector <frequency_bitval> bit_n_count(bit_count);
    
    for(int i(0); i<bit_count; i++){
        if(checked_uncompressed_bits.at(i) == 1)
            continue;
         
        frequency = 0;   
        for(int j(0); j<bit_count; j++){
            if(uncompressed_bits[i] == uncompressed_bits[j]){
              
                checked_uncompressed_bits.at(j) = 1;
                frequency++;
            }    
        }
        
        bit_n_count.at(i) = {frequency ,uncompressed_bits.at(i)} ;
         
    }


    /*============sorting the vector with the bit pattern and frequency=======================*/
    int rows = bit_n_count.size(); 
    sort(bit_n_count.begin(), bit_n_count.end(),sortcol); 
    
    
    /*============creating a map wich includes 8 most frequent bit patterns in frequncy descending order*/
    
    for(int i = (rows-1); i >= (rows-8); i--){
        Dictionary[(79-i)] = bit_n_count.at(i).bit;
    }

    
    /*===========compression=========================*/
    
    
    
    
    
    /*========reading all the compressed bit lines from an input text file and storing them in a vector========= */
    ifstream compressed_file {"compressed.txt"};
    string c_line{};
    string all_bits{};

    if(!compressed_file){
        cerr << "File was not opened correctly. error" << endl;
        return 1;
    }
    
    int p (0);
    while (!compressed_file.eof()){
        getline(compressed_file , c_line);
        if(c_line != "xxxx"){
            compressed_bits.push_back (c_line);
        } 
        p++;

    }
    compressed_file.close();

    
    int compressed_vector_len = compressed_bits.size() -1;
    vector <string> Dictionary_index {"000" , "001" , "010" , "011" , "100" , "101" , "110" , "111" };
    vector <string> Dictionary_vector(8);
        for(int k = (compressed_vector_len); k >= (compressed_vector_len-7); k--){
        Dictionary_vector.at(compressed_vector_len-k) = compressed_bits.at(k);    
    }

    vector <string> all_bit_lines (1);
    for(int l(0) ; l < (compressed_vector_len -7) ; l++){
        all_bits = all_bits + compressed_bits.at(l);
    }
    all_bit_lines.at(0) = all_bits;

    /*************************************************************************
     *                            Decompression                              *
     *************************************************************************/
    

    decompression (all_bit_lines ,Dictionary_index , Dictionary_vector);
    cout << Decompresssed_bitpat.at(0) << endl;
  
    
    return (0); 
}
    
