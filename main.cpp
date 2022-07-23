#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Structure to store driver's data
struct DriverData {
  std::string vehicle_registration_number;
  int driver_age;
};

// Extracting driver's data from input
void getDriverData(DriverData &driverData, std::string line) {
  // Storing spaces index to extract driver data
  std::vector<int> index;
  for (int i = 0; i < line.length(); i++) {
    if (line[i] == ' ') {
      index.push_back(i);
    }
  }
  driverData.vehicle_registration_number =
      line.substr(index[0]+1, index[1] - index[0] - 1);
  driverData.driver_age = stoi(line.substr(index[2]+1, line.length() - index[2]));
}

// Find maximum parking slot size
int findParkingLotSize(std::string line) {
  std::string max_parking_slots = "";
  for (int i = line.find(" ") + 1; i < line.length(); i++) {
    max_parking_slots += line[i];
  }
  return stoi(max_parking_slots);
}

//  Find nearest empty slot available for parking
int findNearestEmptySlot(std::vector<bool> &parking_slots) {
  for (int i = 0; i < parking_slots.size(); i++) {
    if (parking_slots[i] == false) {
      parking_slots[i] = true;
      return i + 1;
    }
  }
  return -1;
}

int main() {
  // File Stream to open input.txt file to read input
  std::ifstream ifs;
  ifs.open("input.txt", std::ios::in);

  // Storing Parking Data
  std::map<int, std::pair<std::string, int>> parkingData;

  // Storing nearest slot available for parking
  int nearest_slot = -1;

  // Storing maximum number of parking slots
  int max_parking_slots;

  // String to store input data from line
  std::string line;

  // Reading one line from input.txt
  getline(ifs, line);

  // Storing maximum number of Parking slots
  max_parking_slots = findParkingLotSize(line);

  // Vector to store the parking slots availability : true - slot filled, false
  // - slot empty
  std::vector<bool> parking_slots(max_parking_slots, false);

  // Printing output for the command
  std::cout << "Created parking of " << max_parking_slots << " slots\n";

  // Reading input line by line
  while (getline(ifs, line)) {
    // Park new driver
    if (line.find("Park") != std::string::npos &&
        line.find("driver_age") != std::string::npos) {
      DriverData driverData;
      getDriverData(driverData, line);
      nearest_slot = findNearestEmptySlot(parking_slots);
      if (nearest_slot != -1) {
        // storing parking data
        // parkingData[{driverData.vehicle_registration_number,driverData.driver_age}]
        // = nearest_slot;
        parkingData[nearest_slot] = {driverData.vehicle_registration_number,
                                     driverData.driver_age};
        std::cout << "Car with vehicle registration number "
                  << "\"" << parkingData[nearest_slot].first << "\""
                  << " has been parked at slot number " << parkingData[nearest_slot].second << "\n";
      } else {
        std::cout << "No Empty Slots Available\n";
      }
    }

    // Slot_numbers_for_driver_of_age
    if (line.find("Slot_numbers_for_driver_of_age") != std::string::npos) {
      // finding and storing age of driver
      int driver_age = stoi(line.substr(line.find(" ") + 1, line.length() - line.find(" ")));
      std::vector<int> slot_numbers_for_driver_age;
      for (auto i : parkingData) {
        if (i.second.second == driver_age) {
          slot_numbers_for_driver_age.push_back(i.first);
        }
      }
      for (int i = 0; i < slot_numbers_for_driver_age.size(); i++) {
        if (i < slot_numbers_for_driver_age.size() - 1)
          std::cout << slot_numbers_for_driver_age[i] << ",";
        else
          std::cout << slot_numbers_for_driver_age[i];
      }
      std::cout << "\n";
    }

    // Slot_number_for_car_with_number
    if (line.find("Slot_number_for_car_with_number") != std::string::npos) {
      // Finding and storing vehicle registration number
      std::string vehicle_registration_number = 
        line.substr(line.find(" ") + 1, line.length() - line.find(" "));
      for (auto i : parkingData) {
        if (i.second.first == vehicle_registration_number) {
          std::cout << i.first << "\n";
          break;
        }
      }
    }

    // Driver is leaving with his car
    if (line.find("Leave") != std::string::npos) {
      int leave_slot =
          stoi(line.substr(line.find(" ") + 1, line.length() - line.find(" ")));
      if (parking_slots[leave_slot - 1] == false) {
        std::cout << "Slot already empty\n";
      } else {
        parking_slots[leave_slot - 1] = false;
        DriverData driverData;
        std::cout << "Slot number " << leave_slot << " vacated,"
                  << " the car with vehicle registration number \""
                  << parkingData[leave_slot].first << "\""
                  << "left the space,"
                  << " the driver of the car was of age "
                  << parkingData[leave_slot].second << "\n";
        parkingData.erase(leave_slot);
      }
    }

    // Vehicle_registration_number_for_driver_of_age
    if (line.find("Vehicle_registration_number_for_driver_of_age") !=
        std::string::npos) {
      int driver_age =
          stoi(line.substr(line.find(" ") + 1, line.length() - line.find(" ")));
      std::vector<std::string> vehicle_registration_number_for_driver_age;
      for (auto i : parkingData) {
        if (i.second.second == driver_age) {
          vehicle_registration_number_for_driver_age.push_back(i.second.first);
        }
      }
      for (int i = 0; i < vehicle_registration_number_for_driver_age.size();
           i++) {
        if (i < vehicle_registration_number_for_driver_age.size() - 1)
          std::cout << vehicle_registration_number_for_driver_age[i] << ",";
        else
          std::cout << vehicle_registration_number_for_driver_age[i];
      }
      std::cout << "\n";
    }
  }
}