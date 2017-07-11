//
//  GTF.cpp
//  IntervalTree
//
//  Created by Aaron Graubert on 6/28/17.
//  Copyright © 2017 Aaron Graubert. All rights reserved.
//

#include "GTF.h"
#include <sstream>
#include <exception>
#include <stdexcept>

using std::ifstream;
using std::string;
using std::map;

const string EXON_NAME = "exon";
const int U_SHORT_MAX = 65535u;
map<string, unsigned short> chromosomes;

//std::map<unsigned short, int> chromosomeMap = std::map<unsigned short, int>();

ifstream& operator>>(ifstream &in, Feature &out)
{
    try{
        string line;
        while(getline(in, line))
        {
            if(line[0] == '#') continue; //not a feature line
            std::istringstream tokenizer(line);
            //get chr#
            string buffer;
            if(!getline(tokenizer, buffer, '\t')) throw std::length_error("invalid GTF line: "+line);
            /*if(buffer.size() >=3 && buffer[0] == 'c' && buffer[1] == 'h')
            {
                out.chromosome = chromosomeMap(buffer.substr(3));
            }
            else out.chromosome = chromosomeMap(buffer);*/
            out.chromosome = chromosomeMap(buffer);
            //get track name
            if(!getline(tokenizer, buffer, '\t')) throw std::length_error("invalid GTF line: "+line);
            //get feature type
            if(!getline(tokenizer, buffer, '\t')) throw std::length_error("invalid GTF line: "+line);
            out.type = buffer;
            //get start pos
            if(!getline(tokenizer, buffer, '\t')) throw std::length_error("invalid GTF line: "+line);
            out.start = std::stoull(buffer);
            //get stop pos
            if(!getline(tokenizer, buffer, '\t')) throw std::length_error("invalid GTF line: "+line);
            out.end = std::stoull(buffer);
            //get score
            if(!getline(tokenizer, buffer, '\t')) throw std::length_error("invalid GTF line: "+line);
            //get strand
            if(!getline(tokenizer, buffer, '\t')) throw std::length_error("invalid GTF line: "+line);
            switch(buffer[0])
            {
                case '+':
                    out.strand = 1;
                    break;
                case '-':
                    out.strand = -1;
                    break;
                default:
                    out.strand = 0;
            }
            //get frame
            if(!getline(tokenizer, buffer, '\t')) throw std::length_error("invalid GTF line: "+line);
            //get attributes
            if(!getline(tokenizer, buffer, '\t')) throw std::length_error("invalid GTF line: "+line);
            std::map<string, string> attributes;
            parseAttributes(buffer, attributes);
            if (attributes.find("gene_id") != attributes.end()) out.gene_id = attributes["gene_id"];
            if (attributes.find("transcript_id") != attributes.end()) out.transcript_id = attributes["transcript_id"];
            if (attributes.find("exon_id") != attributes.end()) out.exon_id = attributes["exon_id"];
            if (attributes.find("transcript_type") != attributes.end()) out.transcript_type = attributes["transcript_type"];
            break;
        }

    }
    catch(std::exception &e)
    {
        std::cout<<"Here's what happened: "<<e.what()<<std::endl;
    }
    return in;
}

unsigned short chromosomeMap(std::string chr)
{
    auto entry = chromosomes.find(chr);
    if (entry == chromosomes.end())
    {
        chromosomes[chr] = chromosomes.size() + 1u;
        std::cout << chr << " -> " << chromosomes.size() << std::endl;
    }
    return chromosomes[chr];
}

std::map<std::string,std::string>& parseAttributes(std::string &intake, std::map<std::string,std::string> &attributes)
{
    std::istringstream tokenizer(intake);
    string buffer;
    while (getline(tokenizer, buffer, ';'))
    {
        std::istringstream splitter(buffer);
        string current;
        getline(splitter, current, '"');
        string key = current.substr(0, current.length()-1);
        while (key[0] == ' ') key = key.substr(1);
        getline(splitter, current, '"');
        attributes[key] = current;
    }
    return attributes;
}
