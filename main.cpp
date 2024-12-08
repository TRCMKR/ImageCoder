#include "Codings.h"
#include "Cyphers.h"
#include "Coder.h"
#include "Steganography.h"

#include "suppFuncs.h"
#include "suppClasses.h"
#include "labFuncs.h"

int main() {
    srand(time(NULL));

    /// LAB 1
//    lab1();

    /// LAB 2
//    lab2();

    /// LAB 3
//    lab3();

//    auto C = generateCode(5, 3, 2);
//    vec_vec_GF2 basis = findBasis(to_mat_GF2(C), 3, 2);
//    vector<matrix> vec = toVector(basis, 3);
//    cout << vec[0] << endl;
    /// LAB 4
//    lab4();
//    string Im = "../images/cat.bmp";
//    string originalData = pixelsToString(Im);
//    LFSRCypher cypher("../lab2/values 2.txt");
//    vec_vec_GF2 C = generateCode(15, 5, 5);
//    vector<matrix> vecG = toVector(findBasis(to_mat_GF2(C), 5, 7), 5);
//    auto G = vecG[1];
//
//    auto compressionMap = toHaffman(Im);
////    cout << originalData << endl;
//    auto X = encode(Im, compressionMap);
//    int countX = 5 - X.size() % 5;
//    X = encrypt(X + string(countX, '0'), cypher);
//    X = encodeData(X, G);
//
//    auto Y = encrypt(originalData, cypher);
//    int countY = 5 - Y.size() % 5;
//    Y = encodeData(Y + string(countY, '0'), G);
//
//    string name = "../images/big poodle.bmp";
//
//    string nameEmbeded1 = embedLSB(name, X, "_copy1");
//    auto S1 = pixelsToString(nameEmbeded1);
//    string nameEmbeded2 = embedLSBPM(name, Y, "_copy2");
//    auto S2 = pixelsToString(nameEmbeded2);
//
//    cout << '\n' << "1:" << endl;
//    vector<double> freqs = {0.5, 0.1, 0.01, 0.001};
//    cout << "Im1: " << '\n';
//    for (auto q: freqs) {
//        BSC channel(q);
//        cypher("../lab2/values 2.txt");
//        cout << "\tFOR Q = " << q << '\n';
//        string Im1 = decodeData(extractFromString(channel(S1), X.size()), G, 10);
//        Im1 = decrypt(Im1, cypher).substr(0, Im1.size() - countX);
//        Im1 = decode(Im1, compressionMap);
//        cout << '\t';
//        printComparison(Im1, originalData, 10);
//    }
//
//    cout << "Im2: " << '\n';
//    for (auto q: freqs) {
//        BSC channel(q);
//        cypher("../lab2/values 2.txt");
//        cout << "\tFOR Q = " << q << '\n';
//        string Im2 = extractFromString(channel(S1), X.size());
//        Im2 = decrypt(Im2, cypher);
//        Im2 = decodeData(Im2, G, 10).substr(0, Im2.size() - countX);
//        Im2 = decode(Im2, compressionMap);
//        cout << '\t';
//        printComparison(Im2, originalData, 10);
//    }
//
//    cout << "2:" << endl;
//    cout << "Im1: " << '\n';
//    for (auto q: freqs) {
//        BSC channel(q);
//        cypher("../lab2/values 2.txt");
//        cout << "\tFOR Q = " << q << '\n';
//        string Im1 = decodeData(extractFromString(channel(S2), Y.size()), G, 10);
//        Im1 = decrypt(Im1, cypher).substr(0, Im1.size() - countX);
//        cout << '\t';
//        printComparison(Im1, originalData, 10);
//    }
//
//    cout << "Im2: " << '\n';
//    for (auto q: freqs) {
//        BSC channel(q);
//        cypher("../lab2/values 2.txt");
//        cout << "\tFOR Q = " << q << '\n';
//        string Im2 = extractFromString(channel(S2), Y.size());
//        Im2 = decrypt(Im2, cypher);
//        Im2 = decodeData(Im2, G, 10).substr(0, Im2.size() - countX);
//        cout << '\t';
//        printComparison(Im2, originalData, 10);
//    }

//    string extractedData = extract(newName, codedData.size());
//    cout << "ENC CYP COMP EXTR = " << extractedData << endl;
//    string decodedExtrData = decodeData(extractedData, G, 20);
//    cout << "CYP COMP EXTR = " << decodedExtrData << endl;
//    cypher("../lab2/values.txt");
//    string decypheredDecExtrData = decrypt(decodedExtrData, cypher);
//    cout << "COMP EXTR = " << decypheredDecExtrData << endl;
//    string originalData = decode(decypheredDecExtrData.substr(1104), compressionMap);
//    cout << "EXTR = " << decypheredDecExtrData.substr(0, 1104) + originalData << endl;

//    saveBMPFile("../images/gotcha.bmp", originalData, 50, 50);
//    cout << toByte("01001101") << endl;

//    auto code = to_mat_GF2(findBasis(to_mat_GF2(generateCode(17, 10, 4)), 10));
//    cout << code << '\n';

    matrix m = {{1, 1, 1, 1, 1, 1, 1, 1}, {1, 0, 1, 0, 1, 0, 1, 0},
                {1, 1, 0, 0, 1, 1, 0, 0}, {1, 1, 1, 1, 0, 0, 0, 0}};

    matrix extH = {
            {1, 0, 1, 0, 1, 0, 1, 0},
            {0, 1, 1, 0, 0, 1, 1, 0},
            {0, 0, 0, 1, 1, 1, 1, 0},
            {1, 1, 1, 1, 1, 1, 1, 1}
    };

//    cout << 0x123213213AC349 << endl;

    // cout << m * transpose(m) << endl;
//    cout << transpose(extH) * extH << endl;
//    cout << linearSpan(toVect(extH));

    matrix z = {
        {1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1},
            {1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
            {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0}
    };

    matrix g = {
        {1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0},
            {1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1},
            {0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0},
            {0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1},
            {1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1}
    };
    int t = 2;

    auto res = ISD(z, g, t, 100);
    for (long i = 0; i < res.NumRows(); ++i) {
        cout << res[i] << endl;
    }

    auto c = res * g;
    for (long i = 0; i < c.NumRows(); ++i) {
        cout << c[i] << endl;
    }
    return 0;
}
