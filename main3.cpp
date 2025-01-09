#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <limits>

using namespace std;

//Basic Syntax & Data Types//
enum class RiskLevel { LOW, MEDIUM, HIGH };
enum class VehicleType { CAR, TRUCK, MOTORCYCLE };

//Abstraction//
class InsuranceRisk {
protected:
    virtual double calculateBaseRisk() const = 0;
    virtual double applyModifiers(double risk) const = 0;
public:
    virtual ~InsuranceRisk() = default;
    virtual double calculateTotalRisk() const = 0;
};

//Inheritance//
class Vehicle {
private:
    string make, model;
    int year, accidentCount;
    VehicleType type;
    bool isCommercial;
public:
    Vehicle(string m, string mod, int y, VehicleType t, int acc = 0, bool comm = false)
        : make(m), model(mod), year(y), type(t), accidentCount(acc), isCommercial(comm) {}
    string getMake() const { return make; }
    string getModel() const { return model; }
    int getYear() const { return year; }
    VehicleType getType() const { return type; }
    int getAccidentCount() const { return accidentCount; }
    bool getIsCommercial() const { return isCommercial; }
};

class CarRiskCalculator : public InsuranceRisk {
private:
    const Vehicle& vehicle;
    static constexpr double BASE_RISK = 0.8;
protected:
    double calculateBaseRisk() const override { return BASE_RISK * (vehicle.getIsCommercial() ? 1.5 : 1.0); }
    double applyModifiers(double risk) const override {
        return risk * (1.0 + (2024 - vehicle.getYear()) * 0.05) * (1.0 + vehicle.getAccidentCount() * 0.15);
    }
public:
    explicit CarRiskCalculator(const Vehicle& v) : vehicle(v) {}
    double calculateTotalRisk() const override {
        return applyModifiers(calculateBaseRisk());
    }
};

//Factory pattern & smart pointer//
class RiskCalculatorFactory {
public:
    static unique_ptr<InsuranceRisk> createCalculator(const Vehicle& vehicle) {
        return unique_ptr<InsuranceRisk>(new CarRiskCalculator(vehicle));
    }
};

// STL//
class InsuranceRiskSystem {
private:
    map<string, RiskLevel> riskCategories;
    
    /* TOPIC: Control Flow & Functions */
    RiskLevel categorizeRisk(double score) {
        return score < 1.2 ? RiskLevel::LOW : (score < 1.8 ? RiskLevel::MEDIUM : RiskLevel::HIGH);
    }

public:
    void assessVehicleRisk(const Vehicle& vehicle) {
        auto calculator = RiskCalculatorFactory::createCalculator(vehicle);
        double risk = calculator->calculateTotalRisk();
        string vehicleId = vehicle.getMake() + " " + vehicle.getModel();
        riskCategories[vehicleId] = categorizeRisk(risk);
    }

    void printRiskAssessment() const {
        for (const auto& risk : riskCategories) {
            string riskLevelStr;
            switch (risk.second) {
                case RiskLevel::LOW:
                    riskLevelStr = "LOW";
                    break;
                case RiskLevel::MEDIUM:
                    riskLevelStr = "MEDIUM";
                    break;
                case RiskLevel::HIGH:
                    riskLevelStr = "HIGH";
                    break;
            }
            cout << "Vehicle: " << risk.first << "\nRisk Level: " 
                     << riskLevelStr << "\n";
        }
    }
};

//function to get valid vehicle type input
VehicleType getVehicleTypeInput() {
    int typeChoice;
    cout << "Select Vehicle Type:\n";
    cout << "1. Car\n2. Truck\n3. Motorcycle\n";
    cout << "Enter your choice (1-3): ";
    
    while (true) {
        cin >> typeChoice;
        
        // Clear input stream in case of invalid input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter 1, 2, or 3: ";
            continue;
        }
        
        switch (typeChoice) {
            case 1: return VehicleType::CAR;
            case 2: return VehicleType::TRUCK;
            case 3: return VehicleType::MOTORCYCLE;
            default:
                cout << "Invalid choice. Please enter 1, 2, or 3: ";
        }
    }
}

//function to get yes/no input
bool getYesNoInput(const string& prompt) {
    char response;
    cout << prompt << " (y/n): ";
    
    while (true) {
        cin >> response;
        
        // Clear input stream
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        // Convert to lowercase
        response = tolower(response);
        
        if (response == 'y') return true;
        if (response == 'n') return false;
        
        cout << "Invalid input. Please enter y or n: ";
    }
}

//Exception Handling//
int main() {
    try {
        InsuranceRiskSystem system;
        char continueInput;

        do {
            // Get vehicle make
            string make, model;
            cout << "Enter vehicle make: ";
            getline(cin, make);

            // Get vehicle model
            cout << "Enter vehicle model: ";
            getline(cin, model);

            // Get vehicle year
            int year;
            cout << "Enter vehicle year: ";
            while (!(cin >> year) || year < 1970 || year > 2024) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid year. Please enter a year between 1900 and 2024: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Get vehicle type
            VehicleType type = getVehicleTypeInput();

            // Get accident count
            int accidentCount;
            cout << "Enter number of accidents: ";
            while (!(cin >> accidentCount) || accidentCount < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a non-negative number: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Check if commercial vehicle
            bool isCommercial = getYesNoInput("Is this a commercial vehicle");

            // Create vehicle and assess risk
            Vehicle vehicle(make, model, year, type, accidentCount, isCommercial);
            system.assessVehicleRisk(vehicle);

            // Print risk assessment
            system.printRiskAssessment();

            // Ask if user wants to continue
            continueInput = getYesNoInput("Do you want to assess another vehicle") ? 'y' : 'n';
            cout << endl;

        } while (continueInput == 'y');

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}