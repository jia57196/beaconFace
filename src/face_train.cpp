/*
Copyright (c) 2016, Taiga Nomi
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of the <organization> nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <glob.h>
#include <vector>
#include <string>
#include <dirent.h>
#include <boost/filesystem.hpp>
#include <tiny_cnn/tiny_cnn.h>

#include "common.h"

using namespace tiny_cnn;
using namespace tiny_cnn::activation;
using namespace std;

void convert_image(const std::string& imagefilename,
                   double minv,
                   double maxv,
                   int w,
                   int h,
                   vec_t& data) {
    auto img = cv::imread(imagefilename, cv::IMREAD_GRAYSCALE  );
    if (img.data == nullptr) {
        std::cerr << "cannot open, or it's not an image..." << endl;
        return; // cannot open, or it's not an image
    }

    cv::Mat_<uint8_t> resized;
    cv::resize(img, resized, cv::Size(w, h));

    // mnist dataset is "white on black", so negate required
    std::transform(resized.begin(), resized.end(), std::back_inserter(data),
    [ = ](uint8_t c) { return (255 - c) * (maxv - minv) / 255.0 + minv; });
}


template <typename N>
void construct_net(N& nn) {
    typedef convolutional_layer<activation::identity> conv;
    typedef max_pooling_layer<relu> max_pool;

    const int n_fmaps = 64; ///< number of feature maps for upper layer
    const int n_fmaps2 = 128; ///< number of feature maps for lower layer
    const int n_fc = 64; ///< number of hidden units in fully-connected layer

    nn << conv(112, 112, 7, 1, n_fmaps, padding::same)
       << max_pool(112, 112, n_fmaps, 2)
       << conv(56, 56, 7, n_fmaps, n_fmaps, padding::same)
       << max_pool(56, 56, n_fmaps, 2)
       << conv(28, 28, 7, n_fmaps, n_fmaps2, padding::same)
       << max_pool(28, 28, n_fmaps2, 2)
       << conv(14, 14, 7, n_fmaps2, n_fmaps2, padding::same)
       << max_pool(14, 14, n_fmaps2, 2)
       << fully_connected_layer<activation::identity>(7 * 7 * n_fmaps2, n_fc)
       << fully_connected_layer<softmax>(n_fc, 10);
}

void train_cifar10(string data_dir_path, string test_dir_path, double learning_rate, ostream& log) {
    // specify loss-function and learning strategy
    network<sequential> nn;
    adam optimizer;

    construct_net(nn);

    log << "learning rate:" << learning_rate << endl;

    cout << "load models..." << endl;

    // load cifar dataset
    vector<label_t> train_labels, test_labels;
    vector<vec_t> train_images, test_images;


    // convert imagefile to vec_t
    std::vector<path> ret;
    get_all_files(data_dir_path, ".jpg", ret);

    std::cout << "train image path is:" << data_dir_path << ", " << ret.size()
              << " of images" << std::endl;

    for (size_t it = 0; it < ret.size(); it++) {
        uint8_t label = (uint8_t) it;
        train_labels.push_back(label);
        vec_t data;
        std::string strPath = ret[it].string();
        convert_image(strPath, -1.0, 1.0, 112, 112, data);
        std::cout << "train image is converted: " << strPath << std::endl;
        train_images.push_back(data);
    }

    ret.clear();
    get_all_files(test_dir_path, ".jpg", ret);
    for (size_t it = 0; it < ret.size(); it++) {
        uint8_t label = (uint8_t) it;
        test_labels.push_back(label);
        vec_t data;
        std::string strPath = ret[it].string();
        convert_image(strPath, -1.0, 1.0, 112, 112, data);
        std::cout << "test image is converted: " << strPath << std::endl;
        test_images.push_back(data);
    }

    progress_display disp(ret.size());
    timer t;
    const int n_minibatch = 10; ///< minibatch size
    const int n_train_epochs = 30; ///< training duration

    optimizer.alpha *= static_cast<tiny_cnn::float_t>(sqrt(n_minibatch) * learning_rate);

    // create callback
    auto on_enumerate_epoch = [&]() {
        cout << t.elapsed() << "s elapsed." << endl;
        tiny_cnn::result res = nn.test(test_images, test_labels);
        log << res.num_success << "/" << res.num_total << endl;

        disp.restart(train_images.size());
        t.restart();
    };

    auto on_enumerate_minibatch = [&]() {
        disp += n_minibatch;
    };

    cout << "start training...." << endl;
    // training
    nn.train<cross_entropy>(optimizer, train_images, train_labels,
                            n_minibatch, n_train_epochs, on_enumerate_minibatch, on_enumerate_epoch);

    cout << "end training." << endl;

    // test and show results
    nn.test(test_images, test_labels).print_detail(cout);

    // save networks
    ofstream ofs("cifar-weights");
    ofs << nn;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        cerr << "Usage : " << argv[0]
             << "arg[0]: path_to_data (example:../data/112)"
             << "arg[1]: path_to_test (example:../data/112/test)"
             << "arg[2]: learning rate (example:0.01)" << endl;
        return -1;
    }
    train_cifar10(argv[1], argv[2], stod(argv[3]), cout);
}
