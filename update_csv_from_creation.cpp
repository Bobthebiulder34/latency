#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct Sample {
    int cpu, gpu, ssd;
};

std::vector<Sample> parse_creation_txt(const std::string& path) {
    std::ifstream infile(path);
    std::vector<Sample> samples;
    std::string line;
    std::vector<int> values;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string label, usage;
        int value;
        iss >> label >> usage >> value;
        values.push_back(value);
        if (values.size() == 3) {
            samples.push_back({values[0], values[1], values[2]});
            values.clear();
        }
    }
    return samples;
}

void write_sample_csv(const std::vector<Sample>& samples, const std::string& path) {
    std::ofstream outfile(path);
    outfile << "metric";
    for (size_t i = 0; i < samples.size(); ++i) {
        outfile << "," << (i+1)*5 << " minutes";
    }
    outfile << "\n";
    outfile << "cpu";
    for (const auto& s : samples) outfile << "," << s.cpu;
    outfile << "\n";
    outfile << "gpu";
    for (const auto& s : samples) outfile << "," << s.gpu;
    outfile << "\n";
    outfile << "ssd";
    for (const auto& s : samples) outfile << "," << s.ssd;
    outfile << "\n";
}

int main() {
    std::string creation_path = "creation.txt";
    std::string csv_path = "sample.csv";
    auto samples = parse_creation_txt(creation_path);
    write_sample_csv(samples, csv_path);
    std::cout << "Updated " << csv_path << " with " << samples.size() << " samples.\n";
    return 0;
}
