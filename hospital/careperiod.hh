/* Class CarePeriod
 * 
 * Class for describing a patient's care period in hospital.
 *
 *
 * */

#ifndef CAREPERIOD_HH
#define CAREPERIOD_HH

#include "date.hh"
#include "person.hh"
#include <string>
#include <set>

const std::string CARE_PERIOD = "* Care period: ";
const std::string SPACER = " - ";
const std::string STAFF = "  - Staff:";
const std::string NO_STAFF = " None";
const std::string PATIENT = "* Patient: ";

class CarePeriod {
public:
    // Constructor, start date given as a string (ddmmyyyy).
    CarePeriod(const std::string& start, Person* patient);

    // Constructor, start date given as a Date object.
    CarePeriod(const Date& start, Person* patient);

    // Destructor.
    ~CarePeriod();

    // More public methods
    void set_end_date(const Date& end);
    void add_assignee(std::string staff_id);
    void print();
    void print_period()const;
    void print_patient()const;
    bool has_staff(const std::string& staff_id);
    Person* get_patient()const;

private:
    Person* patient_;
    Date start_;
    Date end_;

    // More attributes and methods
    std::set<std::string> assignees_;

};

#endif // CAREPERIOD_HH
