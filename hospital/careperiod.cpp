#include "careperiod.hh"
#include <iostream>

CarePeriod::CarePeriod(const std::string& start, Person* patient)
        : patient_(patient)
        , start_(start)
{
}

CarePeriod::CarePeriod(const Date& start, Person* patient)
        : patient_(patient)
        , start_(start)
{
}

CarePeriod::~CarePeriod()
{
}

void CarePeriod::set_end_date(const Date& end)
{
    end_ = end;
}

void CarePeriod::add_assignee(std::string staff_id)
{
    // Add staff to the assignees
    assignees_.insert(staff_id);
}

bool CarePeriod::has_staff(const std::string& staff_id)
{
    for (auto assignee : assignees_) {
        if (assignee == staff_id) {
            return true;
        }
    }
    return false;
}

void CarePeriod::print()
{
    // Print care period
    std::cout << CARE_PERIOD;
    print_period();

    // Check if patient has any assignees
    std::cout << STAFF;
    if (assignees_.empty()) {
        std::cout << NO_STAFF << std::endl;
        return;
    }

    // Print assignees
    for (auto staff : assignees_) {
        std::cout << " " << staff;
    }
    std::cout << std::endl;
}

Person* CarePeriod::get_patient()const{
    return patient_;
}

void CarePeriod::print_period()const {
    start_.print();
    std::cout << SPACER;
    if (!end_.is_default()) {
        end_.print();
    }
    std::cout << std::endl;
}

void CarePeriod::print_patient()const {
    std::cout << PATIENT;
    get_patient()->print_id();
    std::cout << std::endl;
}
