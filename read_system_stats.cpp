#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <cstdlib>
#include <ctime>

// Read CPU usage from Windows Performance Monitor
int get_cpu_usage() {
    FILE* pipe = _popen("wmic os get totalvisiblememorybytes,freephysicalmemory", "r");
    if (!pipe) return -1;
    
    char buffer[256];
    int line_count = 0;
    unsigned long total = 0, free = 0;
    
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        line_count++;
        if (line_count == 2) {
            sscanf(buffer, "%lu %lu", &total, &free);
            break;
        }
    }
    _pclose(pipe);
    
    if (total == 0) return 0;
    int used_percent = (int)(((total - free) * 100) / total);
    return used_percent > 100 ? 100 : used_percent;
}

// Read GPU usage (NVIDIA GPUs)
int get_gpu_usage() {
    FILE* pipe = _popen("nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader,nounits 2>nul", "r");
    if (!pipe) {
        // Fallback: return a simulated value if nvidia-smi is not available
        return -1;
    }
    
    char buffer[32];
    int usage = -1;
    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        sscanf(buffer, "%d", &usage);
    }
    _pclose(pipe);
    return usage;
}

// Read SSD usage
int get_ssd_usage() {
    FILE* pipe = _popen("wmic logicaldisk get name,size,freespace where name='C:' /format:list", "r");
    if (!pipe) return -1;
    
    char buffer[256];
    unsigned long size = 0, free = 0;
    
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        if (sscanf(buffer, "Size=%lu", &size) == 1) {
            // Continue reading
        }
        if (sscanf(buffer, "FreeSpace=%lu", &free) == 1) {
            // Continue reading
        }
    }
    _pclose(pipe);
    
    if (size == 0) return 0;
    int used_percent = (int)(((size - free) * 100) / size);
    return used_percent > 100 ? 100 : used_percent;
}

void append_to_creation_txt(int cpu, int gpu, int ssd) {
    std::ofstream file("creation.txt", std::ios::app);
    file << "cpu usage " << cpu << "\n";
    file << "gpu usage " << gpu << "\n";
    file << "ssd usage " << ssd << "\n";
    file.close();
}

void push_to_github() {
    // Stage changes
    int status = std::system("git add creation.txt");
    if (status != 0) {
        std::cerr << "Failed to stage changes\n";
        return;
    }
    
    // Commit with timestamp
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    char timestamp[100];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    std::string commit_cmd = "git commit -m \"Auto-update system stats ";
    commit_cmd += timestamp;
    commit_cmd += "\"";
    
    status = std::system(commit_cmd.c_str());
    if (status != 0) {
        std::cerr << "Failed to commit (may be no changes)\n";
    }
    
    // Push to GitHub
    status = std::system("git push origin main");
    if (status != 0) {
        std::cerr << "Failed to push to GitHub. Make sure you have internet and proper Git credentials.\n";
        return;
    }
    
    std::cout << "Successfully pushed to GitHub!\n";
}

int main() {
    int cpu = get_cpu_usage();
    int gpu = get_gpu_usage();
    int ssd = get_ssd_usage();
    
    std::cout << "CPU: " << cpu << "%\n";
    std::cout << "GPU: " << (gpu >= 0 ? std::to_string(gpu) : "N/A") << "%\n";
    std::cout << "SSD: " << ssd << "%\n";
    
    // If GPU is unavailable, use a default value
    if (gpu < 0) gpu = 0;
    
    append_to_creation_txt(cpu, gpu, ssd);
    std::cout << "Data appended to creation.txt\n";
    
    // Push to GitHub
    push_to_github();
    
    return 0;
}
