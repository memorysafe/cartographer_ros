/*
 * Author: Jianxiong Cai
 * Description:
 *      read in submap pbstream file and convert into PGM file
 *      output to the same directory containing pbstream
 */

#include <fstream>
#include <iostream>
#include <string>

#include "submap_converter.h"

#include "cartographer/mapping/proto/map_builder_options.pb.h"
#include "cartographer/mapping/map_builder.h"

using namespace std;

void print_usage(){
    cout << "Usage: rosrun cartographer_submap_converter -d { path to directory containing submap-x.pbstream}";
}

int main(int argc, char *argv[]) {

    for (int i = 0; i < 17; i ++) {
        string map_filename = "/home/ernest/playground/submap-" + std::to_string(i) + ".pbstream";
        string pgm_filename = "/home/ernest/playground/submap-" + std::to_string(i) + ".pgm";
        SubmapConverter submap_converter(map_filename);
        submap_converter.toPGM(pgm_filename);
    }
    return 0;
}
