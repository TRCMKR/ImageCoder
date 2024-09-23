#include "Codings.h"
#include "Cyphers.h"
#include "Coder.h"

void lab1() {
    string fileName = "../comparesFile.txt";
    ofstream file(fileName, ios::app);
    vector<pair<int, string>> data;
    string name = "../Source Images/30x20.bmp";

//    file.close();
//    file.open(fileName);
//    name = "../Source Images/30x20.bmp";
//    data = compare(name, toFano);
//    printLnData(data, "\n", file, 100, 1);
//    data = compare(name, toHuffman);
//    printLnData(data, "\n", file, 100, 2);
//
//    name = "../Source Images/Инжир.bmp";
//    data = compare(name, toFano);
//    printLnData(data, "\n", file, 100, 1);
//    data = compare(name, toHuffman);
//    printLnData(data, "\n", file, 100, 2);
//
//    name = "../Source Images/Малина.bmp";
//    data = compare(name, toFano);
//    printLnData(data, "\n", file, 100, 1);
//    data = compare(name, toHuffman);
//    printLnData(data, "\n", file, 100, 2);
//
//    CImg<unsigned char> img(name.c_str());
//    int width = img.width();
//    int height = img.height();
//    for (int r = 0; r < height; r++)
//        for (int c = 0; c < width; c++) {
//            cout << "(" << (int)img(c,r,0,0) << ", " << (int)img(c,r,0,1) << ", " << (int)img(c,r,0,2) << ") ";
//        }
//
//    cout << endl;
//    string outputName = "../result.txt";
//    saveToTxt(name, outputName, toFano);
//    decode(outputName);
//
//    vector<vector<int>> myFIO { {1040, 3}, {1048, 3}, {32, 2}, {1045, 2}, {1053, 2}, {1056, 2}, {1057, 2}, {1042, 1}, {1043, 1}, {1044, 1}, {1050, 1}, {1051, 1}, {1054, 1}, {1055, 1}, {1063, 1}, {1071, 1} };
//    data = toFano(myFIO, 25);
//    printLnData(data);
//    printLnMaxElem(data);
//    cout << seqLength(data, myFIO) << '\n';
//
//    data = toHuffman(myFIO, 25);
//    printLnData(data);
//    printLnMaxElem(data);
//    cout << seqLength(data, myFIO) << '\n';
}

void lab2() {
//    LFSR reg;
//    reg.data = { {1, 1}, {1, 0}, {1, 0}, {1, 1} };
//    reg.printLn();
//    cout << (reg >> 1) << '\n';
//    reg.printLn();
//    cout << '\n';
//
//    string name = "../values.txt";
//    LFSRCypher cypher(name);
//    cypher.printLn();
//
//    string fileToEncrypt = "../result.txt";
//    encrypt(fileToEncrypt, cypher, true);
//    std::filesystem::path p(fileToEncrypt);
//    string newName = p.stem().string() + "_encrypted" + p.extension().string();
//    newName = p.remove_filename().string() + newName;
//    decrypt(newName, cypher);
//
//    string imageName = "../Source Images/30x20.bmp";
//    string pixelDataName = pixelsToTxt(imageName);
//    string encName = encrypt(pixelDataName, cypher);
//    string decName = decrypt(encName, cypher);
//
//    string comparesFile = "../Source Images/comparesCypher.txt";
//    auto data = compareCypher(imageName, cypher);
//    ofstream cypherFile(comparesFile);
//    printLnData(data, "\n", cypherFile, 100);
//    cypherFile.close();
//
//    data = compare(imageName, toHaffman);
//    cypherFile.open(comparesFile, ios::app);
//    printLnData(data, "\n", cypherFile, 100);
}

void lab3() {
    string name = "../images/cat.bmp";
    auto encodedData = encode(name, toHaffman(name));
//    cout << encodedData << endl;
    LFSRCypher cypher("../lab2/values.txt");
    auto data = encrypt(encodedData, cypher);
    if (data.size() % 5 != 0)
        data += string(5 - data.size() % 5, '0');
    cout << "ORIGINAL DATA = " << data << endl;

//    matrix mat;
//    mat.SetDims(25, 15);
//    mat = { {1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0},
//            {0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            {1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0},
//            {1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1},
//            {0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0},
//            {1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0},
//            {1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0},
//            {0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//            {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1},
//            {1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1},
//            {0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
//            {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1},
//            {0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0},
//            {1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
//            {0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1},
//            {1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1},
//            {0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0},
//            {0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0},
//            {1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0},
//            {0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
//            {0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0},
//            {0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1},
//            {0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0},
//            {1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0},
//            {0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1} };

    vec_vec_GF2 C = generateCode(15, 5, 5);
    vec_vec_GF2 basis = findBasis(to_mat_GF2(C), 5, 7);
    vector<matrix> vec = toVector(basis, 5);
    vector<matrix> h = toCheckVector(vec);
//    cout << projectMatr(msg, t) << endl;
//    cout << vec[1] << endl;
//    cout << h[0] << endl;
//    cout << vec[2] * transpose(h[7]) << endl;

    matrix msg({{1, 0, 1, 1, 1}});
    matrix z = msg * vec[0];
    matrix err = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1}};
//    cout << z << endl;
//    cout << ISD(z + err, vec[0], 2, 6) << endl;

    string codedData = encodeData(data, vec[1]);
    string corruptedCodedData = corruptData(codedData, vec[1].NumCols());
    cout << "CODED AND CORRUPTED DATA = " << corruptedCodedData << endl;
    vector<int> iters = {1, 3, 6, 10};
    vector<double> freqs = {0.5, 0.1, 0.01, 0.001};
//    cout << std::setprecision(12) << std::fixed;
    cout << "FOR CORRUPTED EVERY 15BIT\n";
    printLnComparison(corruptedCodedData, data, vec[1], iters);
    cout << "FOR BSC\n";
    for (auto q: freqs) {
        BSC channel(q);
        cout << "FOR Q = " << q << '\n';
        printLnComparison(channel(codedData), data, vec[1], iters);
    }

//    BSC channel(0.000000001);
//    string someData = string((long)10e8, '1');
//    string modifiedData = channel(someData);
//    cout << std::count_if(modifiedData.begin(), modifiedData.end(), [](const auto & x){ return x == '0'; }) << endl;

//    string recoveredData = decodeData(coruptedCodedData, vec[1], 6);
//    cout << recoveredData.size() << endl;
//    recoveredData = decodeData(coruptedCodedData, vec[1], 1);
//    cout << recoveredData.size() << endl;

}

void lab4() {

//    string secretName = "../images/cat.bmp";
//    auto secretPixels = pixelsToString(secretName);
//    cout << "REAL DATA = " << secretPixels << '\n';
//    auto compressionMap = toHaffman(secretName);
//    auto encodedData = encode(secretName, compressionMap);
//    string header = copyBMPHeader(secretName);
//    header = header.substr(0, header.size() - (header.size() + encodedData.size()) % 5);
////    cout << header << endl;
////    cout << header.size() / 8 << ' ' << encodedData.size() % 5 << endl;
//    cout << "COMP DATA = " << (header + encodedData) << endl;
//
//
//    LFSRCypher cypher("../lab2/values.txt");
//    auto data = encrypt(header + encodedData, cypher);
//    cout << "CYP COMP DATA = " << data << endl;
//
//    vec_vec_GF2 C = generateCode(15, 5, 5);
//    vector<matrix> vecG = toVector(findBasis(to_mat_GF2(C), 5, 7), 5);
//    vector<matrix> vecH = toCheckVector(vecG);
//    auto G = vecG[1];
//    string codedData = encodeData(data, G);
//    cout << "ENC CYP COMP DATA = " << codedData << endl;
//
//    string name = "../images/big poodle.bmp";
//    string newName = embedLSBPM(name, codedData);
//    auto pixels = pixelsToString(name);
//    auto newPixels = pixelsToString(newName);
////    cout << pixels.substr(0, 100) << '\n';
////    cout << newPixels.substr(10500, 100) << '\n';
//
//    string extractedData = extract(newName, codedData.size());
//    cout << "ENC CYP COMP EXTR = " << extractedData << endl;
//    string decodedExtrData = decodeData(extractedData, G, 20);
//    cout << "CYP COMP EXTR = " << decodedExtrData << endl;
//    cypher("../lab2/values.txt");
//    string decypheredDecExtrData = decrypt(decodedExtrData, cypher);
//    cout << "COMP EXTR = " << decypheredDecExtrData << endl;
//    string originalData = decode(decypheredDecExtrData.substr(1104), compressionMap);
//    cout << "EXTR = " << decypheredDecExtrData.substr(0, 1104) + originalData << endl;
//
//    saveBMPFile("../images/gotcha.bmp", originalData, 50, 50);
////    cout << toByte("01001101") << endl;
}