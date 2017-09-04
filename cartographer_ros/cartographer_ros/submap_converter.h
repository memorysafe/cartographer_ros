//
// Created by ernest on 17-8-30.
//

#ifndef SRC_SUBMAP_READER_H
#define SRC_SUBMAP_READER_H


#include <iostream>
#include <fstream>
#include <glog/logging.h>

#include "cartographer/mapping_2d/submaps.h"

#include "cartographer/mapping/proto/submap.pb.h"

using namespace std;

class SubmapConverter{
public:
    explicit SubmapConverter(string file_name);

    ~SubmapConverter();

    // print it out as PGM
    void toPGM(string file_name);
private:

    ifstream * in_;
    cartographer::mapping_2d::Submap * submap_;
};

#endif //SRC_SUBMAP_READER_H
