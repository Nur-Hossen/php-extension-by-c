/* 
 * File:   samity.cpp
 * Author: amlan
 * 
 * Created on February 6, 2011, 4:57 PM
 */

#include "samity.h"
#include <boost/algorithm/string.hpp>

samity::samity(int id = 0,
        int branch_id = 0,
        int working_area_id = 0,
        int field_officer_id = 0,
        int product_id = 0,
        std::string name = "",
        std::string code = "",
        std::string registration_no = "",
        std::string samity_day = "Sun",
        std::string samity_type = "",
        boost::gregorian::date opening_date = boost::gregorian::day_clock::local_day(),
        int created_by = 0,
        boost::gregorian::date created_date = boost::gregorian::day_clock::local_day(),
        int updated_by = 0,
        boost::gregorian::date updated_date = boost::gregorian::day_clock::local_day(),
        int is_deleted = 0,
        int deleted_by = 0,
        boost::gregorian::date delete_date = boost::gregorian::day_clock::local_day(),
        int status = 0,bool is_samity_day_obsolete=0) {

    std::ofstream fout;
    fout.open("/opt/microfin_v3/application/logs/microfin360/scheduler.log", std::ios::app);
    fout << "Samity ID: " << id << " Created\n";

    this->curr_act_date = boost::gregorian::from_simple_string("1900-01-01");

    week_days["Sun"] = 0;
    week_days["Mon"] = 1;
    week_days["Tue"] = 2;
    week_days["Wed"] = 3;
    week_days["Thu"] = 4;
    week_days["Fri"] = 5;
    week_days["Sat"] = 6;

    samity_schedules = new samity_schedule_type();
    samity_schedules_index = new samity_schedule_index_type();
}

samity::samity() {

    std::ofstream fout;
    fout.open("/var/www/microfin360/trunk/application/logs/microfin360/scheduler.log", std::ios::app);
    fout << "Samity ";

    this->curr_act_date = boost::gregorian::from_simple_string("1900-01-01");

    week_days["Sun"] = 0;
    week_days["Mon"] = 1;
    week_days["Tue"] = 2;
    week_days["Wed"] = 3;
    week_days["Thu"] = 4;
    week_days["Fri"] = 5;
    week_days["Sat"] = 6;

    samity_schedules = new samity_schedule_type();
    samity_schedules_index = new samity_schedule_index_type();

    this->b_holiday = NULL;
    this->s_holiday = NULL;
}

samity::samity(const samity& orig) {

    std::ofstream fout;
    fout.open("/var/www/microfin360/trunk/application/logs/microfin360/scheduler.log", std::ios::app);
    fout << "Copy Samity ID: " << orig.GetId() << " Created\n";

    id = orig.GetId();
    branch_id = orig.GetBranch_id();
    working_area_id = orig.GetWorking_area_id();
    field_officer_id = orig.GetField_officer_id();
    product_id = orig.GetProduct_id();
    name = orig.GetName();
    code = orig.GetCode();
    registration_no = orig.GetRegistration_no();
    is_samity_day_obsolete=orig.is_samity_day_obsolete;
    //samity_days_type temp_samity_days_type;
    /*
        for (it = orig.samity_days.begin(); it != orig.samity_days.end(); ++it) {
            //std::cerr<<"In Samity CP "<<it->first<<" "<<it->second<<std::endl;
            this->samity_days[it->first]=it->second;
            ////exit(1);
        }
     */
    ////exit(1);
    this->samity_days = orig.samity_days;
    this->samity_schedules = orig.samity_schedules;
    this->samity_schedules_index = orig.samity_schedules_index;
    /*
    if(orig.GetId()==649){
    for (it = samity_days.begin(); it != samity_days.end(); ++it) {
            //std::cerr<<"In Samity CP "<<it->first<<" "<<it->second<<std::endl;
            this->samity_days[it->first]=it->second;
        
        }
    //exit(1);
    }
     */

    this->week_days = orig.week_days;


    samity_type = orig.GetSamity_type();
    opening_date = orig.GetOpening_date();
    created_by = orig.GetCreated_by();
    created_date = orig.GetCreated_date();
    updated_by = orig.GetUpdated_by();
    updated_date = orig.GetUpdated_date();
    is_deleted = orig.GetIs_deleted();
    deleted_by = orig.GetDeleted_by();
    delete_date = orig.GetDelete_date();
    status = orig.GetStatus();
    sam_day = orig.sam_day;
    samity_day = orig.samity_day;


    //this->sam_day=orig.get_samity_day();
}

samity::~samity() {

    std::ofstream fout;
    fout.open("/var/www/microfin360/trunk/application/logs/microfin360/scheduler.log", std::ios::app);
    fout << "Samity ID: " << id << " Destroyed\n";
    /*
    if(this->b_holiday){
        this->b_holiday->clear();
        delete(this->b_holiday);
    }
     */
    if (this->s_holiday) {
        this->s_holiday->clear();
        delete(this->s_holiday);
    }

    samity_schedules->clear();
    delete(samity_schedules);

    samity_schedules_index->clear();
    delete(samity_schedules_index);

    week_days.clear();
    samity_days_vector.clear();

}

boost::gregorian::greg_weekday samity::GetSamity_week_day_local(boost::gregorian::date d_date) {
    return boost::gregorian::greg_weekday(week_days[this->get_samity_day(d_date)]);
}

boost::gregorian::greg_weekday samity::GetSamity_week_day(boost::gregorian::date d_date) {
    samity_schedule_type::iterator it = this->samity_schedules->find(d_date);
    if (it != this->samity_schedules->end()) {
        //std::cerr<<" Getsamityday "<<boost::gregorian::greg_weekday(d_date.day_of_week())<<" "<<d_date<<std::endl;
        return boost::gregorian::greg_weekday(d_date.day_of_week());
    }

    return boost::gregorian::greg_weekday(week_days[this->get_samity_day(d_date)]);
}

int samity::GetBranch_id() const {
    return branch_id;
}

void samity::SetBranch_id(int branch_id) {
    this->branch_id = branch_id;
}

std::string samity::GetCode() const {
    return code;
}

void samity::SetCode(std::string code) {
    this->code = code;
}

int samity::GetCreated_by() const {
    return created_by;
}

void samity::SetCreated_by(int created_by) {
    this->created_by = created_by;
}

boost::gregorian::date samity::GetCreated_date() const {
    return created_date;
}

void samity::SetCreated_date(boost::gregorian::date created_date) {
    this->created_date = created_date;
}

boost::gregorian::date samity::GetDelete_date() const {
    return delete_date;
}

void samity::SetDelete_date(boost::gregorian::date delete_date) {
    this->delete_date = delete_date;
}

int samity::GetDeleted_by() const {
    return deleted_by;
}

void samity::SetDeleted_by(int deleted_by) {
    this->deleted_by = deleted_by;
}

int samity::GetField_officer_id() const {
    return field_officer_id;
}

void samity::SetField_officer_id(int field_officer_id) {
    this->field_officer_id = field_officer_id;
}

int samity::GetId() const {
    return id;
}

void samity::SetId(int id) {
    std::ofstream fout;
    fout.open("/var/www/microfin360/trunk/application/logs/microfin360/scheduler.log", std::ios::app);
    fout << "ID: " << id << " Created\n";
    this->id = id;
}

int samity::GetIs_deleted() const {
    return is_deleted;
}

void samity::SetIs_deleted(int is_deleted) {
    this->is_deleted = is_deleted;
}

std::string samity::GetName() const {
    return name;
}

void samity::SetName(std::string name) {
    this->name = name;
}

boost::gregorian::date samity::GetOpening_date() const {
    return opening_date;
}

void samity::SetOpening_date(boost::gregorian::date opening_date) {
    this->opening_date = opening_date;
}

int samity::GetProduct_id() const {
    return product_id;
}

void samity::SetProduct_id(int product_id) {
    this->product_id = product_id;
}

std::string samity::GetRegistration_no() const {
    return registration_no;
}

void samity::SetRegistration_no(std::string registration_no) {
    this->registration_no = registration_no;
}

std::string samity::GetSamity_type() const {
    return samity_type;
}

void samity::SetSamity_type(std::string samity_type) {
    this->samity_type = samity_type;
}

int samity::GetStatus() const {
    return status;
}

void samity::SetStatus(int status) {
    this->status = status;
}

int samity::GetUpdated_by() const {
    return updated_by;
}

void samity::SetUpdated_by(int updated_by) {
    this->updated_by = updated_by;
}

boost::gregorian::date samity::GetUpdated_date() const {
    return updated_date;
}

void samity::SetUpdated_date(boost::gregorian::date updated_date) {
    this->updated_date = updated_date;
}

int samity::GetWorking_area_id() const {
    return working_area_id;
}

void samity::SetWorking_area_id(int working_area_id) {
    this->working_area_id = working_area_id;
}

bool samity::GetIs_samity_day_obsolete() const {
    return is_samity_day_obsolete;
}

void samity::SetIs_samity_day_obsolete(bool is_samity_day_obsolete) {
    this->is_samity_day_obsolete = is_samity_day_obsolete;
}

void samity::add_samity_days(std::string day, std::string act_date) {

    boost::gregorian::date tmp = boost::gregorian::from_simple_string(act_date);
    this->samity_days[tmp] = day;
    samity_days_vector.push_back(tmp);
}

std::string samity::get_samity_day(boost::gregorian::date d_date) {
    std::string day = this->sam_day;
    //std::cerr<<day<<std::endl;
    int diff = 401767;
    for (it = samity_days.begin(); it != samity_days.end(); ++it) {
        //std::cerr<<d_date<<" "<<it->first<<" "<<it->second<<std::endl;
        if (d_date >= it->first && d_date.julian_day() - it->first.julian_day() <= diff && d_date.julian_day() - it->first.julian_day() >= 0) {
            day = it->second;
            diff = d_date.julian_day() - it->first.julian_day();
        }

    }

    return day;  
}

void samity::set_samity_day(std::string sam) {
    this->sam_day = sam;
}


