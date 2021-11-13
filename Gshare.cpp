#include <iostream>
#include <string>
#include <fstream>
#include <math.h> 
#include <vector>

using namespace std;


void Gshare(int, int, string);
unsigned long long int convert(unsigned long long int, int);
char bin_to_hex(unsigned long long int);

void Gshare(int M, int N, string trace_file) {
    string hex;
    unsigned long long int second;
    char actual;
    unsigned long long int address;
    unsigned long long int temp;
    ifstream trace(trace_file);
    int numMisses = 0;
    int numHits = 0;
    int numOperations = 0;
    int count = 0;

    string bin = "";
    string comp = "";
    string fin = "";

    string a = "";
    string b = "";

    int counter = 0;
    
    

    int M_address[M];
    // int second_address[M];
    int global_history_buffer[N];
    int table [int(pow(2, M))];
	if (!trace.is_open()) {
		cout <<  "File could not be opened." << endl;
    }


    for (int i = 0; i < int(pow(2, M)); i++) {
        table[i] = 2;
    }
    for (int i = 0; i < N; i++) {
        global_history_buffer[i] = 0;
    }

    while (true) {
		// // Gets the operations and the address
		trace >> hex >> actual;

        if( trace.eof() ) break;
        // cout << hex << endl;
        address = stoull(hex, 0, 16);
        

        // cout << "Hex: " << hex << "\nint: " << address << endl;

        address /= 4;

        // cout << "Address / 4: " << address << endl;

        temp = address;
        counter = 0;


        while (temp > 0) {
            if (temp % 2 == 0) {
                bin.insert(0,1,'0');
            } else {
                bin.insert(0,1,'1');
            }
            temp /= 2;
            counter++;
        }

        // reverse(bin.begin(),bin.end());

        for (int i = counter -1; i >= (counter - M); i--) {
            comp.insert(0,1,bin[i]);
        }

        // reverse(bin.begin(),bin.end());
        // reverse(comp.begin(),comp.end());

        
        // cout << "Binary (div 2) " << bin << endl;
        // cout << M <<" Digits: " << comp << endl;
        // cout << "Binary -> (div 2) " << stoull(bin, 0, 2) << endl;

        for (int i = 0; i < N; i++) {
            if (comp[i] == '1' && global_history_buffer[i] == 0) {
                fin.insert(0,1,'1');
            }
            else if (comp[i] == '0' && global_history_buffer[i] == 1) {
                fin.insert(0,1,'1');
            } 
            else {
                fin.insert(0,1,'0');
            }
        }
        for (int i = N; i < M; i++) {
            fin += comp[i];
        }


        // cout << fin << endl;

        // reverse(fin.begin(), fin.end());
        // cout << fin << endl;

        counter = 0;
        for (int i = 0; i < M; i++) {
            a.insert(0,1,fin[i]);
            counter++;

            if (counter % 4 == 0) {
                // cout << a << endl;
                char c =  bin_to_hex(stoull(a, 0 , 2));
                b.insert(0,1,c);
                counter = 0;
                a.clear();
            }
        }

        if (counter != 0) {
            // cout << a << endl;
            char c = bin_to_hex(stoull(a, 0 , 2));
            b.insert(0,1,c);
            counter = 0;
            a.clear();
        }
        // cout << b << endl;
        int index = stoull(b, 0, 16);
        index = index % (int)(pow(2, M));
        // cout << index << endl; 


        // Updating Table 
        if (actual == 't') {
            // cout << index << endl;
            if (table[index] < 2) {
                table[index]++;
                numMisses++;
                numOperations++;
            }
            else {
                numHits++;
                numOperations++;
                if (table[index] == 2) {
                    table[index]++;
                }
            }
            // push a zero
            for(int i = N - 1; i >= 0 ; i-- ) {
                global_history_buffer[i+1] = global_history_buffer[i];
            }
            global_history_buffer[0] = 1;
        }

        if (actual == 'n') {
            // cout << index << endl;
            if (table[index] > 1) {
                table[index]--;
                numMisses++;
                numOperations++;
            }
            else {
                numHits++;
                numOperations++;
                if (table[index] == 1) {
                    table[index]--;
                }
            }
            // push a zero
            for(int i = N - 1; i >= 0 ; i-- ) {
                global_history_buffer[i+1] = global_history_buffer[i];
            }
            global_history_buffer[0] = 0;
        }

        //  cout << actual << ": ";
        // for (int i = 0; i < N; i++) {
        //     cout << global_history_buffer[i] << " ";
        // }
        // cout << endl;
        bin = "";
        fin = "";
        comp = "";
        b = "";
        
    }  
    cout << M << " " << N <<  " " << (((double)numMisses / (double)numOperations) * 100) << "%" << endl;
    // cout << "Hits: " << numHits << endl;
    // cout << "Misses: " << numMisses << endl;
    return;
}

char bin_to_hex(unsigned long long int i) {
    if (i < 10) {
        return i + '0';
    }
    else {
        if (i == 10) {
            // cout << 'a' << endl;
            return 'a';
        }
        if (i == 11) {
            // cout << 'b' << endl;
            return 'b';
        }
        if (i == 12) {
            // cout << 'c' << endl;
            return 'c';
        }
        if (i == 13) {
            // cout << 'd' << endl;
            return 'd';
        }
        if (i == 14) {
            // cout << 'e' << endl;
            return 'e';
        }
        if (i == 15) {
            // cout << 'f' << endl;
            return 'f';
        }
    }
    return ' ' ;
}

unsigned long long int convert(unsigned long long int address, int M) {
    vector<int> change;
    int mult = 1;
    unsigned long long int num = 1;

    while (address > 0) {
        change.push_back(address % 10);
        address /= 10;
    }

    for (int i = 0; i < M; i++) {
        num += (mult * change.at(i));
        mult *= 10;
    }

    return num;
}

int main (int argc, char ** argv)
{
	// Block size is a constant and is not affected by CL arguments
    int GPB, RB;
	string trace_file;

	if (argc < 4)
        cout << "Error: Too few arguments" << endl;

	// Sets lines to command args (no error checking, not industry worthy)
	GPB = atoi(argv[1]);
    RB = atoi(argv[2]);
	trace_file = argv[3];

	try
	{
		Gshare(GPB, RB, trace_file);
	}
	catch (const char *e)
	{
		cout << e;
	}

   
    // cout <<  binaryConversion(global_history_buffer) << endl;
    // cout << sizeof(global_history_buffer) << endl;

	return 0;
}