//
// Created by ernest on 17-8-30.
//

#include "submap_converter.h"
SubmapConverter::SubmapConverter(string file_name):in_(nullptr),submap_(nullptr) {
    in_ = new ifstream(file_name);
    if (*in_ == NULL){
        cerr << "[WARNING] Failed to open subamp file: " << file_name << endl;
        return;
    }

    // read in the submap
    cartographer::mapping::proto::SerializedData proto;
    proto.ParseFromIstream(in_);
    auto submap_proto = proto.submap();
    auto submap_2d_proto = submap_proto.submap_2d();

    submap_ = new cartographer::mapping_2d::Submap(submap_2d_proto);
}

SubmapConverter::~SubmapConverter() {
    delete in_;
    delete submap_;
}

void SubmapConverter::toPGM(string file_name) {
    ofstream file_out(file_name);
    if (file_out == NULL){
        cerr << "[WARNING] Failed to print out PGM. file_name: " << file_name << endl;
        throw 6;
    }

    submap_->probability_grid().toPGM(file_out);

}