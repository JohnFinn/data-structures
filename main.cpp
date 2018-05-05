// tests for TreeList
#include <gtest/gtest.h>

#include "TreeList.h"
#include <vector>
#include <iostream>
#include <fstream>


TEST(TreeList_test, insertion){
    TreeList<unsigned long> list;

    // some random values generated by python
    unsigned long indices[] = {
        0,   0, 76,  102, 68,  25,  45,  125, 3,   75, 18, 126, 12,  16,  89,
        65,  47, 125, 62,  122, 63,  65,  114, 13,  92, 71, 65,  0,   125, 58,
        6,   65, 6,   0, 41,  22,  91,  24,  119, 44, 58, 19,  67,  108, 111,
        89,  30, 61,  86,  109, 40,  69,  40,  101, 26, 64, 23,  121, 102, 95,
        11,  10, 17,  24,  10,  53,  125, 100, 81,  22, 68, 86,  97,  98,  59,
        16,  80, 114, 82,  47,  96,  54,  0,  22,  7,  49, 8,   127, 97,  14,
        17,  72, 38,  4,   36,  28,  11,  10,  99,  6,  58, 68,  83,  113, 4,
        113, 96, 125, 7,   120, 56,  5,   123, 6,   39, 58, 1,   26,  98,  3,
        98,  52, 122, 53,  50,  126, 114, 79, 1, 6, 4, 9, 4};
    unsigned long elements[] = {
        94,  26,  100, 22,  41, 27,  47,  63,  114, 95,  29,  60,  63, 31, 75,
        107, 35,  44,  77,  61, 46,  102, 113, 35,  60,  110, 100, 36, 98, 61,
        29,  5,   93,  125, 48, 122, 4,   125, 119, 76,  99,  87,  65, 74, 116,
        118, 123, 89,  29,  5,  83,  57,  0,   89,  11,  98,  108, 88, 17, 71,
        10,  117, 68,  43,  7,  119, 52,  11,  4,   119, 89,  49,  74, 41, 14,
        28,  66,  25,  99,  74, 66,  32,  27,  94,  27,  116, 102, 98, 20, 105,
        25,  51,  4,   75,  12, 121, 17,  70,  113, 75,  101, 33,  87, 6,  71,
        64,  101, 12,  35,  0,  114, 31,  71,  100, 53,  72,  114, 32, 1,  35,
        74,  113, 122, 43,  42, 63,  104, 3, 2, 25, 356, 23, 1};

    unsigned long results[] = {
        27,  2,   114, 125, 1,   356, 35,  36,  25,  26, 23,  71,  31,  100,
        75,  94,  35,  75,  100, 114, 27,  102, 70,  22, 17,  93,  29,  41,
        7,   105, 117, 27,  32,  25,  10,  47,  63,  28, 95,  29,  68,  60,
        94,  121, 63,  35,  119, 53,  31,  87,  75,  43, 107, 12,  35,  42,
        108, 122, 113, 43,  4,   44,  125, 11,  77,  61, 46,  102, 123, 72,
        113, 114, 60,  110, 100, 98,  101, 61,  116, 5,  74,  48,  4,   0,
        3,   83,  119, 33,  76,  32,  99,  65,  119, 74, 116, 118, 89,  14,
        29,  5,   51,  57,  87,  89,  98,  88,  17,  74, 71,  1,   52,  89,
        11,  4,   49,  74,  101, 41,  66,  25,  104, 99, 66,  98,  20,  113,
        6,   64,  12,  0,   71,  122, 63};



    for (int i = 0; i < sizeof(indices) / sizeof(*indices); ++i){
        list.insert(indices[i], elements[i]);
//        std::cout << "```mermaid\ngraph TD\n" << list << "```\n\n";
    }

    for (int i = 0; i < sizeof(indices) / sizeof(*indices); ++i) {
        EXPECT_EQ(list.at(i), results[i]);
        auto node = list.get_node(i);
        EXPECT_EQ(node->height, 1 + std::max(node->bheight(), node->sheight()));
        EXPECT_LE(std::abs(node->slope()), 1);
    }

}


TEST(TreeList_test, insertion2){
    TreeList<int> list;
    std::vector<int> vec;
    vec.push_back(-1); // avoid floating point exception in x % 0 operation
    list.insert(0,-1);
    std::srand(0);
    int N = 2000;
    for (int i = 0; i <= N; ++i){
        unsigned long index = std::rand() % vec.size(); // % size to make more in the middle insertions
        vec.insert(vec.begin()+index, i);
        list.insert(index, i);

        for (int j = 0; j < vec.size(); ++j) {
            EXPECT_EQ(list.at(j), vec.at(j));
            auto node = list.get_node(i);
            EXPECT_EQ(node->height, 1 + std::max(node->bheight(), node->sheight()));
            EXPECT_LE(std::abs(node->slope()), 1);
        }
    }
}


TEST(TreeList_test, deletion){
    std::ofstream fout("test");
    TreeList<int> list;
    // N times randomly insert or delete random element
    // each time check equality

    int N = 5000;
    std::vector<int> vec;
    std::srand(0);
    for (int i = 0; i <= N; ++i){
        list.insert(0, -1);
        fout << "insert(0,-1)\n\n```mermaid\ngraph TD\n" << list << "```\n\n";


        vec.insert(vec.begin(), -1); // avoid FPE
        unsigned int index  = std::rand() % vec.size(); // % size to make more hits in the middle
        if (std::rand() % 2){
            fout << "insert(" << index << ", " << i << ")\n\n";
            vec.insert(vec.begin() + index, i);
            list.insert(index, i);
        } else {
            fout << "remove(" << index << ")\n\n";
            vec.erase(vec.begin() + index);
            list.remove(index);
        }
        if (list.root)
            EXPECT_GE(list.root->diff, 0);
        for (int j = 0; j < vec.size(); ++j) {
            EXPECT_EQ(vec.at(j), list.at(j));
            auto node = list.get_node(j);
            EXPECT_EQ(node->height, 1 + std::max(node->bheight(), node->sheight()));
            EXPECT_LE(std::abs(node->slope()), 1);
        }
        fout << "```mermaid\ngraph TD\n" << list << "```\n\n";
    }


}


TEST(TreeList_test, push_back){
    TreeList<int> list;
    int elements[] = {9,8,7,6,5,4,99,22,3,2};
    for (int i = 0; i < sizeof(elements) / sizeof(*elements); ++i){
        list.push_back(elements[i]);
        std::cout << "```mermaid\ngraph TD\n" << list << "```\n\n";
        for (int j = 0; j <= i; ++j) {
            EXPECT_EQ(elements[j], list.at(j));
            auto node = list.get_node(j);
            EXPECT_EQ(node->height, 1 + std::max(node->bheight(), node->sheight()));
            EXPECT_LE(std::abs(node->slope()), 1);
        }

    }
}


TEST(TreeList_test, push_back2){
//    std::freopen("test", "w", stdout);
    TreeList<int> list;
    std::vector<int> vec;
    int N = 2000;
    for (int i = 0; i <= N; ++i){
        list.push_back(i);
        vec.push_back(i);
//        std::cout << "```mermaid\ngraph TD\n" << list << "```\n\n";
        for (int j = 0; j <= i; ++j) {
            EXPECT_EQ(vec.at(j), list.at(j));
            auto node = list.get_node(j);
            EXPECT_EQ(node->height, 1 + std::max(node->bheight(), node->sheight()));
            EXPECT_LE(std::abs(node->slope()), 1);
        }

    }
}

TEST(TreeList_test, move_left){
    // not proper test
    TreeList<int> list;
    typedef TreeList<int>::Node Node;
    list.push_back(1);
    Node n = *list.root;
    EXPECT_EQ(list.move_left(1), nullptr);
    EXPECT_EQ(*list.root, n);
    auto result = list.move_left(0);
    EXPECT_EQ(result, list.root);
    EXPECT_EQ(*result, n);
}


TEST(TreeList_test, root_deletion){
    TreeList<int> list;
    std::vector<int> vec;
    for (int i = 0; i < 100; ++i) {
        list.push_back(i);
        vec.push_back(i);
    }

    while (list.root) {
        auto index = list.root->diff;
        list.remove(index);
        vec.erase(vec.begin() + index);
        for (int j = 0; j < vec.size(); ++j) {
            EXPECT_EQ(vec.at(j), list.at(j));
            auto node = list.get_node(j);
            EXPECT_EQ(node->height, 1 + std::max(node->bheight(), node->sheight()));
            EXPECT_LE(std::abs(node->slope()), 1);
        }
        std::cout << "```mermaid\ngraph TD\n" << list << "```\n\n";
    }
}


int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
