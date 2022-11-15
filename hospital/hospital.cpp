#include "hospital.hh"
#include "utility.hh"
#include <algorithm>
#include <iostream>

Hospital::Hospital()
{
}

Hospital::~Hospital()
{
    // Deallocating staff
    for (std::map<std::string, Person*>::iterator
             iter
         = staff_.begin();
         iter != staff_.end();
         ++iter) {
        delete iter->second;
    }

    // Deallocating patients
    for (auto patient : all_patients_id) {
        delete get_care_period(patient)->get_patient();
    }

    // Deallocating care periods
    for (auto care_period : care_periods_) {
        delete care_period;
    }
}

void Hospital::recruit(HosPeop hosPeop)
{
    std::string specialist_id = hosPeop.at(0);
    if (staff_.find(specialist_id) != staff_.end()) {
        std::cout << ALREADY_EXISTS << specialist_id << std::endl;
        return;
    }
    Person* new_specialist = new Person(specialist_id);
    staff_.insert({ specialist_id, new_specialist });
    std::cout << STAFF_RECRUITED << std::endl;
}

void Hospital::enter(HosPeop hosPeop)
{
    // Get pateients id
    std::string patient_id = hosPeop.at(0);

    // Check if patient is not currently in hospital
    if (current_patients_.find(patient_id) != current_patients_.end()) {
        std::cout << ALREADY_EXISTS << patient_id << std::endl;
        return;
    }

    // Check if patient had a previous care period
    CarePeriod* care_period = get_care_period(patient_id);
    // If patient hadn't visited hospital (no care period),
    // create a new patient object, else get patient from prev care period
    Person* patient = care_period == nullptr ? new Person(patient_id) : care_period->get_patient();
    if (care_period == nullptr) {
        all_patients_id.insert(patient_id);
    }

    // Add patient to hospital list
    current_patients_.insert({ patient_id, patient });
    // Get the current date
    Date today = utility::today;
    // Initialize patient new care period
    CarePeriod* new_care_record = new CarePeriod(today, patient);

    // Add to care records
    care_periods_.push_back(new_care_record);

    std::cout << PATIENT_ENTERED << std::endl;
}

/**
* @brief Function that implements discharging a patient from the hospital
* @param HosPeop - vector string containing the patient's id
*
*/
void Hospital::leave(HosPeop hosPeop)
{
    std::string patient_id = hosPeop.at(0);

    // Check if patient in hospital
    if (current_patients_.find(patient_id) == current_patients_.end()) {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }

    // Patient in hospital

    // Get the current date
    Date today = utility::today;
    // End patient current care plan
    get_care_period(patient_id)->set_end_date(today);
    // Remove patient from hospital current patients
    current_patients_.erase(patient_id);
    std::cout << PATIENT_LEFT << std::endl;
}

/**
* @brief Function that implements assigning of staff to a patient
* @param HosPeop - vector string containing the staff's id & patient's id
*
*/
void Hospital::assign_staff(HosPeop hosPeop)
{
    std::string specialist_id = hosPeop.at(0);
    std::string patient_id = hosPeop.at(1);

    // Check if staff exists
    if (staff_.find(specialist_id) == staff_.end()) {
        std::cout << CANT_FIND << specialist_id << std::endl;
        return;
    }

    // Check if patient is not in hospital
    if (current_patients_.find(patient_id) == current_patients_.end()) {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }

    std::cout << STAFF_ASSIGNED << patient_id << std::endl;

    // Get access to the latest patient care plan
    CarePeriod* current_care_period = get_care_period(patient_id);

    // Check if patient has already been assigned the staff
    if (current_care_period->has_staff(specialist_id)) {
        return;
    }

    // Add staff to patient assignee list
    current_care_period->add_assignee(specialist_id);
}

void Hospital::add_medicine(HosPeop hosPeop)
{
    std::string medicine = hosPeop.at(0);
    std::string strength = hosPeop.at(1);
    std::string dosage = hosPeop.at(2);
    std::string patient = hosPeop.at(3);
    if (not utility::is_num(strength, true) or not utility::is_num(dosage, true)) {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    std::map<std::string, Person*>::const_iterator
        patient_iter
        = current_patients_.find(patient);
    if (patient_iter == current_patients_.end()) {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }
    patient_iter->second->add_medicine(medicine, stoi(strength), stoi(dosage));

    // Remove patient from medicine list
    remove_medicine_patient(patient, medicine);

    // Check if medicine is in hospital list
    if (medicines_.find(medicine) == medicines_.end()) {
        // Create new medicine
        medicines_.insert({ medicine, { patient } });
    } else {
        // Add patient to existing medicine
        medicines_.at(medicine).insert(patient);
    }

    std::cout << MEDICINE_ADDED << patient << std::endl;
}

void Hospital::remove_medicine(HosPeop hosPeop)
{
    std::string medicine = hosPeop.at(0);
    std::string patient = hosPeop.at(1);
    std::map<std::string, Person*>::const_iterator
        patient_iter
        = current_patients_.find(patient);
    if (patient_iter == current_patients_.end()) {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }
    patient_iter->second->remove_medicine(medicine);
    remove_medicine_patient(patient, medicine);
    std::cout << MEDICINE_REMOVED << patient << std::endl;
}

void Hospital::print_patient_info(HosPeop hosPeop)
{
    std::string patient_id = hosPeop.at(0);

    CarePeriod* latest_care_period = get_care_period(patient_id);

    if (latest_care_period == nullptr) {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }

    for (auto care_period : care_periods_) {
        if (care_period->get_patient()->get_id() == patient_id)
            care_period->print();
    }

    std::cout << PRINT_MEDICINE;
    latest_care_period->get_patient()->print_medicines(PRE_TEXT);
}

void Hospital::print_care_periods_per_staff(HosPeop hosPeop)
{
    const std::string staff_id = hosPeop.at(0);

    // Check if staff exists
    if (staff_.find(staff_id) == staff_.end()) {
        std::cout << CANT_FIND << staff_id << std::endl;
        return;
    }

    bool has_care_period = false;
    // Loop through each period and check if staff is assigned
    for (auto care_period : care_periods_) {
        if (care_period->has_staff(staff_id)) {
            has_care_period = true;
            care_period->print_period();
            care_period->print_patient();
        }
    }

    // Check if staff has no care record
    if (!has_care_period) {
        std::cout << NONE << std::endl;
    }
}

void Hospital::print_all_medicines(HosPeop)
{
    // Check if their is a medicine in the hospital
    if (medicines_.empty()) {
        std::cout << NONE << std::endl;
        return;
    }
    // loop through the medicine and print the patients using it
    for (auto medicine : medicines_) {
        std::cout << medicine.first << " prescribed for" << std::endl;
        for (auto patient : medicine.second) {
            std::cout << "* " << patient << std::endl;
        }
    }
}

void Hospital::print_all_staff(HosPeop)
{
    if (staff_.empty()) {
        std::cout << NONE << std::endl;
        return;
    }
    for (std::map<std::string, Person*>::const_iterator iter = staff_.begin();
         iter != staff_.end();
         ++iter) {
        std::cout << iter->first << std::endl;
    }
}

void Hospital::print_all_patients(HosPeop)
{
    if (all_patients_id.empty()) {
        std::cout << NONE << std::endl;
        return;
    }
    for (auto patient_id : all_patients_id) {
        std::cout << patient_id << std::endl;
        print_patient_info(std::vector<std::string> { patient_id });
    }
}

void Hospital::print_current_patients(HosPeop)
{
    if (current_patients_.empty()) {
        std::cout << NONE << std::endl;
        return;
    }
    for (auto patient : current_patients_) {
        std::string patient_id = patient.first;
        std::cout << patient_id << std::endl;
        print_patient_info(std::vector<std::string> { patient_id });
    }
}

void Hospital::set_date(HosPeop hosPeop)
{
    std::string day = hosPeop.at(0);
    std::string month = hosPeop.at(1);
    std::string year = hosPeop.at(2);
    if (not utility::is_num(day, false) or not utility::is_num(month, false) or not utility::is_num(year, false)) {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utility::today.set(stoi(day), stoi(month), stoi(year));
    std::cout << "Date has been set to ";
    utility::today.print();
    std::cout << std::endl;
}

void Hospital::advance_date(HosPeop hosPeop)
{
    std::string amount = hosPeop.at(0);
    if (not utility::is_num(amount, true)) {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utility::today.advance(stoi(amount));
    std::cout << "New date is ";
    utility::today.print();
    std::cout << std::endl;
}

CarePeriod* Hospital::get_care_period(const std::string& patient_id)
{
    for (auto it = care_periods_.rbegin(); it != care_periods_.rend(); ++it) {
        if ((*it)->get_patient()->get_id() == patient_id) {
            return *it;
        }
    }
    return nullptr;
}

void Hospital::remove_medicine_patient(std::string patient, std::string pill)
{

    // Search through all the medicine
    for (auto medicine = medicines_.begin(); medicine != medicines_.end(); ++medicine) {
        // Search through all the patients in each medicine
        for (auto i = medicine->second.begin(); i != medicine->second.end(); ++i) {
            // Check if the id matches the patient
            if ((*i) == patient && pill == medicine->first) {
                // Remove the patient from the list
                medicine->second.erase(i);
                // Check if list is now empty
                if (medicine->second.empty()) {
                    // remove medicine from hospital list
                    medicines_.erase(medicine->first);
                }
                // Set exit loop to true
                return;
            }
        }
    }
}
