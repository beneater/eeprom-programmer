#include <iostream>
#include <fstream>
#include <vector>
#include <boost/asio.hpp>

using namespace boost::asio;

// Function to read binary data from a file
std::vector<uint8_t> read_binary_file(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);

    if (!file) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return {};
    }

    // Read the file contents into a vector
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();
    return buffer;
}

void send_binary_data(serial_port& port, const std::vector<uint8_t>& data) {
    boost::system::error_code ec;
    write(port, buffer(data), ec);

    if (ec) {
        std::cerr << "Error sending data: " << ec.message() << std::endl;
    } else {
        std::cout << "Data sent successfully!" << std::endl;
    }
}

int main() {
    std::string port_name = "COM3";  // Adjust to your Arduino's port
    unsigned int baud_rate = 115200;
    std::string file_path = "data.bin";  // Path to your binary file

    try {
        io_service io;
        serial_port port(io, port_name);
        port.set_option(serial_port_base::baud_rate(baud_rate));
        port.set_option(serial_port_base::character_size(8));
        port.set_option(serial_port_base::parity(serial_port_base::parity::none));
        port.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
        port.set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));

        // Read binary data from file
        std::vector<uint8_t> binary_data = read_binary_file(file_path);

        if (!binary_data.empty()) {
            send_binary_data(port, binary_data);
        } else {
            std::cerr << "No data to send!" << std::endl;
        }

    } catch (boost::system::system_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
