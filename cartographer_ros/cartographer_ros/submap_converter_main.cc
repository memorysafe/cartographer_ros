/*
 * Copyright 2016 The Cartographer Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cartographer_ros/node.h"
#include "cartographer_ros/node_options.h"
#include "cartographer_ros/ros_log_sink.h"
#include "gflags/gflags.h"
#include "tf2_ros/transform_listener.h"

#include "submap_converter.h"

DEFINE_string(configuration_directory, "",
              "First directory in which configuration files are searched, "
                      "second is always the Cartographer installation to allow "
                      "including files from there.");
DEFINE_string(configuration_basename, "",
              "Basename, i.e. not containing any directory prefix, of the "
                      "configuration file.");
DEFINE_string(map_filename, "", "If non-empty, filename of a map to load.");
DEFINE_string(output_path,"","The directory to output the submaps");
DEFINE_bool(
        start_trajectory_with_default_topics, true,
        "Enable to immediately start the first trajectory with default topics.");

namespace cartographer_ros {
    namespace {

        // add a backslash to the path if it is not ended with it.
        void check_path(string& path){
            if (path.back() != '/'){
                path.push_back('/');
            }
        }

        void Run() {
            // load the map
            constexpr double kTfBufferCacheTimeInSeconds = 1e6;
            tf2_ros::Buffer tf_buffer{::ros::Duration(kTfBufferCacheTimeInSeconds)};
            tf2_ros::TransformListener tf(tf_buffer);
            NodeOptions node_options;
            TrajectoryOptions trajectory_options;
            std::tie(node_options, trajectory_options) =
                    LoadOptions(FLAGS_configuration_directory, FLAGS_configuration_basename);

            Node node(node_options, &tf_buffer);

            if (FLAGS_map_filename.empty()) LOG(FATAL) << "no map_filename provided";
            if (FLAGS_output_path.empty()) LOG(FATAL) << "no output_path provided";

            check_path(FLAGS_output_path);
            node.LoadMap(FLAGS_map_filename);
            // map loading complete

            // start converting map
            node.ConvertSubmapsToPGM(FLAGS_output_path);
        }

    }  // namespace
}  // namespace cartographer_ros

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::ParseCommandLineFlags(&argc, &argv, true);

    CHECK(!FLAGS_configuration_directory.empty())
    << "-configuration_directory is missing.";
    CHECK(!FLAGS_configuration_basename.empty())
    << "-configuration_basename is missing.";

    ::ros::init(argc, argv, "cartographer_node");
    ::ros::start();

    cartographer_ros::ScopedRosLogSink ros_log_sink;
    cartographer_ros::Run();
    ::ros::shutdown();
}
