//
// Created by arslan on 18.11.18.
//

#ifndef SHARC_HUFFMAN_HPP
#define SHARC_HUFFMAN_HPP

#include "../global_include.hpp"

#include <queue>
//#include <bitset>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

class INode {
public:
    const int f;

    virtual ~INode() = default;

protected:
    explicit INode(int f) : f(f) {};
};


class InternalNode : public INode {
public:
    INode *const left;
    INode *const right;

    InternalNode(INode* c0, INode* c1) : INode(c0->f + c1->f), left(c0), right(c1) {}
    ~InternalNode() override
    {
        delete left;
        delete right;
    }
};

template <typename DataType>
class LeafNode : public INode {
public:
    const DataType c;

    LeafNode(int f, DataType c) : INode(f), c(c) {}
};

struct NodeCmp
{
    bool operator()(const INode* lhs, const INode* rhs) const { return lhs->f > rhs->f; }
};


typedef std::vector<bool> HuffCode;

template <typename DataType>
using HuffCodeMap = std::unordered_map<DataType, HuffCode>;


template <typename DataType>
class HuffmanEncoder {

public:
    Counts<DataType> freqs;
    HuffCodeMap<DataType> outCodes;

    HuffmanEncoder() = default;
    HuffmanEncoder(Counts<DataType> frequencies) {
        this->freqs = frequencies;
    };

    ~HuffmanEncoder() = default;

    void InitFrequencies(Counts<DataType> frequencies) {
        this->freqs = frequencies;
        this->outCodes.clear();
    };

    void Encode() {
        INode* root = BuildTree(this->freqs);
        GenerateCodes(root, HuffCode());
        delete root;
    };

    INode* BuildTree(const Counts<DataType> &frequencies) {
#ifdef BUILD_WITH_EASY_PROFILER
        EASY_FUNCTION();
#endif


        std::priority_queue<INode*, std::vector<INode*>, NodeCmp> trees;

        for (auto it=frequencies.begin(); it!=frequencies.end(); ++it) {
            trees.push(new LeafNode<DataType>(it->second, it->first));
        }
        while (trees.size() > 1)
        {
            INode* childR = trees.top();
            trees.pop();

            INode* childL = trees.top();
            trees.pop();

            INode* parent = new InternalNode(childR, childL);
            trees.push(parent);
        }
        return trees.top();
    };

    void GenerateCodes(const INode *node, const HuffCode &prefix)
    {
#ifdef BUILD_WITH_EASY_PROFILER
        EASY_FUNCTION();
#endif


        if (const LeafNode<DataType> *lf = dynamic_cast<const LeafNode<DataType> *>(node))
        {
            this->outCodes[lf->c] = prefix;
        }
        else if (const InternalNode *in = dynamic_cast<const InternalNode *>(node))
        {
            HuffCode leftPrefix = prefix;
            leftPrefix.push_back(false);
            GenerateCodes(in->left, leftPrefix);

            HuffCode rightPrefix = prefix;
            rightPrefix.push_back(true);
            GenerateCodes(in->right, rightPrefix);
        }
    };


    void Canonicalize(const std::vector<DataType> &alphabet) {
        std::set<size_t> huffman_keys_lens;
        std::unordered_map<size_t, size_t> huffman_keys_lens_count;

        for(const auto &i: this->outCodes){
            huffman_keys_lens.insert(i.second.size());
            huffman_keys_lens_count[i.second.size()] += 1;
        }

//        auto it1 = huffman_keys_lens.begin();


//        DataType cur_key = alphabet[0];
//        size_t code_len = *it1;
//        size_t code = accumulate(this->outCodes[cur_key].rbegin(), this->outCodes[cur_key].rend(), 0, [](int x, int y) { return (x << 1) + y; });

        std::vector<std::vector<bool>> res;
        res.reserve(alphabet.size());


        size_t cur_code_len = 0;
        size_t cur_code_int = 0;
        size_t prev_code_len = 0;

        size_t cur_symbol_num = 0;

        for(auto it=huffman_keys_lens.begin(); it!=huffman_keys_lens.end(); it++) {
            cur_code_len = *it;
            cur_code_int <<= (cur_code_len-prev_code_len);
            for(size_t i=0; i<huffman_keys_lens_count[cur_code_len]; i++) {
//                std::vector<bool> cur_code_bin(boost::dynamic_bitset<>(cur_code_len, cur_code_int));
                boost::dynamic_bitset<> bits(cur_code_len, cur_code_int);
                std::vector<bool> cur_code_bin(cur_code_len);
                for(size_t j=0; j<cur_code_len; j++) {
                    cur_code_bin[j] = bits[j];
                }

                this->outCodes[alphabet[cur_symbol_num]].swap(cur_code_bin);

                cur_code_int += 1;
                cur_symbol_num += 1;
            }

            prev_code_len = cur_code_len;
        }
    }
};


#endif //SHARC_HUFFMAN_HPP
