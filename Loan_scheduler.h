/* 
 * File:   Loan_scheduler.h
 * Author: Murad
 *
 * Created on February 6, 2011, 12:48 PM
 */

#ifndef LOAN_SCHEDULER_H
#define LOAN_SCHEDULER_H

#include<iostream>
#include "ext/hash_map"
#include"boost/date_time/gregorian/gregorian.hpp"
#include "boost/functional/hash.hpp"
#include "zend.h"
#include "zend_API.h"

#include "loan.h"
#include "samity.h"

/*
namespace boost{namespace gregorian{
    inline std::size_t hash_value(const boost::gregorian::date& b)
    {
        boost::hash<long> hasher;
        return hasher(b.julian_day());
    }
}};
 */



class date_pair {
public:

    date_pair(std::string start, std::string end) {
        this->start = boost::gregorian::from_simple_string(start);
        this->end = boost::gregorian::from_simple_string(end);
    }

    date_pair(boost::gregorian::date start, boost::gregorian::date end) {
        this->start = start;
        this->end = end;
    }

    date_pair() {
        this->start = boost::gregorian::from_simple_string("1900-01-01");
        this->end = boost::gregorian::from_simple_string("1900-01-01");
    }

    date_pair(const date_pair& orig) {
        start = orig.start;
        end = orig.end;
    }

    void update_pre(std::string start) {
        this->start = boost::gregorian::from_simple_string(start);
    }

    void update_curr(std::string end) {
        this->end = boost::gregorian::from_simple_string(end);
    }

    boost::gregorian::date start;
    boost::gregorian::date end;
};

class Loan_scheduler {
public:
    int branch_id;
    int samity_id;
    std::vector<date_pair>::iterator it;

    boost::gregorian::date *system_date;

    int org_flag;

    //typedef __gnu_cxx::hash_map<std::string, boost::gregorian::date> br_hash;

    typedef __gnu_cxx::hash_map<boost::gregorian::date, boost::gregorian::date, boost::hash<boost::gregorian::date> > br_hash;

    br_hash branch_holiday_list;
    br_hash *branch_holiday_list_p;



    typedef __gnu_cxx::hash_map<boost::gregorian::date, int, boost::hash<boost::gregorian::date> > samity_schedule_type;
    samity_schedule_type *samity_schedules;

    typedef __gnu_cxx::hash_map<int, boost::gregorian::date> samity_schedule_index_type;
    samity_schedule_index_type *samity_schedules_index;

    typedef __gnu_cxx::hash_map<int, std::vector<date_pair> > reschedule_hash_type;
    reschedule_hash_type loan_reschedule_list;


    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;
    loan_hash loan_list;

    typedef __gnu_cxx::hash_map<int, samity*> samity_hash;
    samity_hash samity_list;

    typedef __gnu_cxx::hash_map<int, std::vector<boost::gregorian::date> > date_hash;

    typedef __gnu_cxx::hash_map<int, br_hash> samity_date_hash;
    //typedef std::map<int, br_hash> samity_date_hash;
    samity_date_hash samity_holiday_list;
    date_hash samity_day_change_list;


    typedef __gnu_cxx::hash_map<int, br_hash*> samity_date_hash_s;
    samity_date_hash_s *samity_holiday_list_p;

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > > branch_samity_loan_product_hash;
    branch_samity_loan_product_hash branch_samity_loan_product_list;
    branch_samity_loan_product_hash branch_samity_member_loan_list;

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > >branch_samity_loan_hash;
    branch_samity_loan_hash branch_samity_loan_list;

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > branch_loan_hash;
    branch_loan_hash branch_loan_list;



    boost::gregorian::date date_from;
    boost::gregorian::date date_to;

    Loan_scheduler();
    Loan_scheduler(const Loan_scheduler& orig);
    ~Loan_scheduler();

    void post_initialize(long unsigned int start, long unsigned int end);
    bool initialize(boost::gregorian::date *system_date, int branch_id, int samity_id, std::string s_date_from, std::string s_date_to, br_hash* b_holiday, samity_date_hash samity_holiday_date_hash, /*std::vector<loan>u_loans,*/reschedule_hash_type loan_reschedule_list, samity_hash u_samities, loan_hash loan_list, int org_flag);
    bool reinitialize(reschedule_hash_type loan_reschedule_list);
    bool initialize(int branch_id, int samity_id, std::string loan_ids);

    void set_system_date(std::string sys_date) {
        boost::gregorian::date *system_date = new boost::gregorian::date(boost::gregorian::from_simple_string(sys_date));
        //std::cerr<<*system_date<<std::endl;
        if (this->system_date) {
            delete(this->system_date);
        }
        this->system_date = system_date;
    }

    template <class T>
    inline std::string to_string(const T& t) {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    void load_samity_day_list(samity_hash samity_list);
    void load_holiday_list(br_hash* branch_holiday_list, samity_date_hash samity_holiday_list);
    void load_loan_list(loan_hash loan_list);
    void load_samity_day_change(date_hash samity_day_change_list);

    void get_valid_schedule_date(boost::gregorian::date &d_date, int samity_id, int loan_id);
    void get_next_schedule_date(boost::gregorian::date &d_date, int samity_id, std::string repayment_frequency, int loan_id);

    void get_daily_valid_schedule_date(boost::gregorian::date& d_date, int samity_id, int loan_id);
    void get_weekly_valid_schedule_date(boost::gregorian::date& d_date, int samity_id, int loan_id);
    void get_weekly_valid_schedule_date_samity_day(boost::gregorian::date& d_date, int samity_id, int loan_id);
    void get_monthly_valid_schedule_date(boost::gregorian::date& d_date, int samity_id, int loan_id, int ins_no);
    void get_grace_period_wise_valid_schedule_date(boost::gregorian::date& d_date, int samity_id, int loan_id, int ins_no, int grace_period);
    void get_monthly_valid_schedule_date_without_samityday(boost::gregorian::date& d_date, int samity_id, int loan_id, int ins_no);

    int get_loan_week_number_considering_date(boost::gregorian::date d_date, int samity_id, int loan);
    void get_monthly_valid_schedule_date_consider_date(boost::gregorian::date& d_date, int samity_id, int loan_id, int ins_no, int month_date, int month_week,bool is_first_repayment_date=true);
    void get_monthly_valid_schedule_date_consider_specific_date(boost::gregorian::date& d_date, int samity_id, int loan_id, int ins_no, int month_date, int month_week);
	boost::gregorian::date get_date_considering_holiday(int samity_id = 0,boost::gregorian::date d_date = boost::gregorian::day_clock::local_day(),bool is_increment=true);

    void get_yearly_valid_schedule_date(boost::gregorian::date& d_date, int samity_id, int loan_id);

    bool is_valid_schedule_date(int loan_id, std::string s_date) {

        boost::gregorian::date d_date(boost::gregorian::from_simple_string(s_date));

        if (d_date.day_of_week() == this->samity_list[this->loan_list[loan_id]->GetSamity_id()]->GetSamity_week_day(d_date)) {
            return true;
        }
        return false;
    }

    std::string get_samity_day(int loan_id, std::string s_date) {
        boost::gregorian::date d_date = boost::gregorian::from_simple_string(s_date);
        return this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->get_samity_day(d_date);
        //return this->samity_list[this->loan_list[loan_id]->GetSamity_id()]->get_samity_day(boost::gregorian::from_simple_string(s_date));
    }

    void run_scheduler(loan *ln);

    void group_by_branch_samity_product();

    std::string get_first_schedule_date(int loan_id);
    std::string get_last_schedule_date(int loan_id);
    std::string get_schedule_date_by_installment_no(int loan_id, int installment_no);
    zval get_schedule_installment_info_by_installment_no(int loan_id, int installment_no = 1);

    zval get_loan_recoverable_amount(int branch_id, int product_id, std::string date);
    zval get_loan_recoverable_amount_branch_id(int branch_id, std::string s_date_frm, std::string s_date_to = "");

    zval get_branch_wise_loan_recoverable_amount_by_product_id(int branch_id, std::string s_date_frm, std::string s_date_to);

    zval get_branch_wise_loan_recoverable_amount(int branch_id, std::string s_date_frm, std::string s_date_to = NULL);
    zval get_branch_wise_loan_recoverable_amount_consider_fully_paid(int branch_id, std::string s_date_frm, std::string s_date_to);

    zval get_loan_installment_information_by_loan_id_trasaction_amount(int loan_id, double total_transaction_amount);

    zval get_loan_installment_information_by_loan_id_trasaction_amount_previous_transaction_amount(int loan_id, double current_transaction_amount, double previous_total_transaction_amount, double previous_total_principle_amount, double previous_total_interest_amount);

    std::vector<double> divide_transaction_to_installment_principle_interest_installment_no(int loan_id, double total_transaction_amount);
    //zval* get_loan_recoverable_amount_by_branch_id(int branch_id,std::string s_date_frm,std::string s_date_to);

    zval get_loan_schedules(int branch_id = 0, int samity_id = 0, int member_id = 0, int loan_id = 0);
    zval get_loan_schedules_samity_wise(int branch_id = 0, int samity_id = 0);
    zval get_full_loan_schedules(int branch_id, int samity_id, int member_id, int loan_id);
    zval get_loan_schedules_by_memeberlist_branch_product(std::vector<int> member_list, int branch_id, int product_id, int samity_id, std::string s_date_frm = "", std::string s_date_to = "");
    zval get_active_member_loan_schedule_info(std::vector<int> member_list, std::string s_date, int branch_id, int product_id, int samity_id = 0);
    zval get_member_wise_current_loan_information_just_before_date(std::vector<int> member_list, std::string s_date_frm, int branch_id, int product_id, int samity_id = 0, std::string s_date_to = "");
    zval get_member_wise_loan_schedules_by_branch(std::vector<int> member_list, int branch_id, std::string s_date_frm = NULL, std::string s_date_to = NULL);

    zval get_loan_transaction_amount(int loan_id, std::string s_date = "");
    zval get_loan_transaction_amount_before_date(int loan_id, std::string s_date);

    zval get_samity_wise_expired_loan_information_before_date(std::vector<int> samity_list, std::string s_date, int branch_id, int product_id = 0);
    zval get_samity_wise_expired_loan_information_between_date(std::vector<int> samity_list, std::string s_date_from, std::string s_date_to, int branch_id, int product_id = 0);
    zval get_samity_wise_current_loan_information_before_date(std::vector<int> samity_list, std::string s_date, int branch_id, int product_id = 0);
    zval get_samity_wise_current_loan_information_between_date(std::vector<int> samity_list, std::string s_date_from, std::string s_date_to, int branch_id, int product_id);
    zval get_samity_wise_current_loan_information_just_before_date(std::vector<int> samity_list, std::string s_date, int branch_id = 0, int product_id = 0, int is_report = 1);
    zval get_samity_wise_current_loan_information_just_before_date_consider_fully_paid(std::vector<int> samity_list, std::string s_date, int branch_id = 0, int product_id = 0);


    zval get_overdue_loans_by_branch_samity(std::vector<int> samity_list, std::string s_date, int branch_id, int product_id = 0);
    zval get_branch_wise_loan_information_before_date(int branch_id, std::string s_date, std::vector<int> samity_list = std::vector<int>(), int product_id = 0, int loan_id = 0, bool include_fully_paid = true);

    zval get_branch_wise_loan_information_between_date(int branch_id, std::string s_date_frm, std::string s_date_to = "");

    zval get_loan_principle_interrest_distribution_by_loan_id_trasaction_amount(int loan_id, double total_transaction_amount);

    zval get_loan_principle_interrest_distribution_by_loan_ids_trasaction_amounts(std::vector<int> loan_ids, std::vector<double> total_transaction_amounts);

    zval get_loan_advance_due_by_loan_ids_trasaction_amounts(std::vector<int> loan_ids, std::vector<double> total_transaction_amounts, std::string s_date, bool include_expired);
    zval get_loan_advance_due_by_loan_ids_trasaction_principal_amounts(std::vector<int> loan_ids, std::vector<double> total_transaction_amounts, std::string s_date, bool include_expired);
    zval get_loan_advance_due_by_loan_ids_trasaction_amounts_between_date(std::vector<int> loan_ids, std::vector<double> total_transaction_before_from_date, std::vector<double> total_transaction_before_to_date, std::string s_date_from, std::string s_date_to, bool include_expired, std::vector<double> principle_total_transaction_before_from_date, std::vector<double> interest_total_transaction_before_from_date, std::vector<double> principle_total_transaction_before_to_date, std::vector<double> interest_total_transaction_before_to_date);

    zval get_max_penalty_by_loan_ids(std::vector<int> loan_ids);
    zval get_max_rebate_by_loan_ids(std::vector<int> loan_ids, std::vector<int> no_of_installment_paid);
    zval get_max_waiver_by_loan_ids(std::vector<int> loan_ids, std::vector<int> no_of_installment_paid);
    void one_time_loan_calculation(loan *ln, boost::gregorian::date schedule_date, int count);

    void get_valid_schedule_date_consider_transfer(boost::gregorian::date& d_date, int samity_id, int loan_id) {

        int flag = 0;
        for (std::vector<date_pair>::iterator it = loan_reschedule_list[loan_id].begin(); it != loan_reschedule_list[loan_id].end(); it++) {
            if (d_date >= it->start && d_date <= it->end) {
                d_date = it->end;
                flag = 1;
                break;
            }
        }

        while ((this->branch_holiday_list.find(d_date) != this->branch_holiday_list.end())) {
            d_date = d_date + boost::gregorian::days(1);
            flag = 1;
        }
        if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end())
            while (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end()) {
                d_date = d_date + boost::gregorian::days(1);
                flag = 1;
            }
        d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));
        d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));
    }

    void get_daily_valid_schedule_date_consider_transfer(boost::gregorian::date& d_date, int samity_id, int loan_id) {

        d_date = d_date + boost::gregorian::days(1);
        for (it = loan_reschedule_list[loan_id].begin(); it != loan_reschedule_list[loan_id].end(); it++) {
            if (d_date >= it->start && d_date <= it->end) {
                d_date = it->end; //+ boost::gregorian::days(1);
                break;
            }
        }
        while ((this->branch_holiday_list.find(d_date) != this->branch_holiday_list.end())) {
            d_date = d_date + boost::gregorian::days(1);
        }
        if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end()) {
            while (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end()) {
                d_date = d_date + boost::gregorian::days(1);
            }
        }
    }

    void get_weekly_valid_schedule_date_consider_transfer(boost::gregorian::date& d_date, int samity_id, int loan_id) {

        d_date = d_date + boost::gregorian::days(1);
        d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));
        d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));
        for (it = loan_reschedule_list[loan_id].begin(); it != loan_reschedule_list[loan_id].end(); it++) {
            if (d_date >= it->start && d_date <= it->end) {
                d_date = it->end; //+ boost::gregorian::days(1);
                break;
            }
        }
        while ((this->branch_holiday_list.find(d_date) != this->branch_holiday_list.end())) {
            d_date = d_date + boost::gregorian::days(1);
            d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));
            d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));
        }
        if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end()) {
            while (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end()) {
                d_date = d_date + boost::gregorian::days(1);
                d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));
                d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));
            }
        }
    }

    void get_monthly_valid_schedule_date_consider_transfer(boost::gregorian::date& d_date, int samity_id, int loan_id, int ins_no) {


        d_date = d_date + boost::gregorian::months(ins_no);
        d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));

        for (it = loan_reschedule_list[loan_id].begin(); it != loan_reschedule_list[loan_id].end(); it++) {
            if (d_date >= it->start && d_date <= it->end) {
                d_date = it->end; //+ boost::gregorian::days(1);
                d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));
                break;
            }
        }
        while ((this->branch_holiday_list.find(d_date) != this->branch_holiday_list.end())) {

            d_date = d_date + boost::gregorian::days(1);
            d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));

        }
        if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end())
            while (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end()) {

                d_date = d_date + boost::gregorian::days(1);
                d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));

            }



    }

    void get_yearly_valid_schedule_date_consider_transfer(boost::gregorian::date& d_date, int samity_id, int loan_id) {


        d_date = d_date + boost::gregorian::years(1);
        d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));

        for (it = loan_reschedule_list[loan_id].begin(); it != loan_reschedule_list[loan_id].end(); it++) {
            if (d_date >= it->start && d_date <= it->end) {
                d_date = it->end; //+ boost::gregorian::days(1);
                d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));
                break;
            }
        }

        while ((this->branch_holiday_list.find(d_date) != this->branch_holiday_list.end())) {

            d_date = d_date + boost::gregorian::days(1);
            d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));

        }
        if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end())
            while (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end()) {
                d_date = d_date + boost::gregorian::days(1);
                d_date = boost::gregorian::next_weekday(d_date, this->samity_list[this->loan_list[loan_id]->GetSamity_id_consider_transfer(d_date)]->GetSamity_week_day(d_date));

            }


    }

    void get_next_schedule_date_consider_transfer(boost::gregorian::date& d_date, int samity_id, std::string repayment_frequency, int loan_id) {
        this->get_valid_schedule_date_consider_transfer(d_date, samity_id, loan_id);
    }

    void run_scheduler_consider_transfer(loan *ln) {
        int count = 1;
        bool is_rescheduled = false;
        boost::gregorian::date schedule_date = ln->GetFirst_repayment_date();
        this->get_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId());
        //std::cerr<<"First Installment Date "<<schedule_date<<std::endl;
        double payable_amount = 0;
        int samity_id = ln->GetSamity_id_consider_transfer(schedule_date);
        int loan_id = ln->GetId();
        //int no_of_ins = ln->GetNumber_of_installment();
        int no_of_ins = ln->GetActualNumber_of_installment();


        if (ln->GetLoan_type().compare("O") == 0) {
            //std::cerr<<"In One Time Loan "<<this->system_date<<"\n";
            ////exit(1);
            if (ln->GetRepayment_frequency().compare("O") == 0) {
                //schedule_date=schedule_date+boost::gregorian::years(1);
                //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
                ln->addNewOneTimeSchedule_date(schedule_date);
            } else {
                schedule_date = ln->GetDisburse_date() + boost::gregorian::days(1);
                this->get_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());
                //std::cerr << "Disburse Date " << schedule_date << std::endl;
                samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
                samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;

                std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];
                schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                //std::cerr << "First Installment Date " << schedule_date << std::endl;
                int week_count = (*samity_schedules)[schedule_date];
                //std::cerr << "After Week Count " << week_count << std::endl;
                boost::gregorian::date last_date = ln->GetFirst_repayment_date();
                if ((*this->system_date) < ln->GetFirst_repayment_date())
                    last_date = (*this->system_date);
                //std::cerr<<"Samity_week_day "<<this->samity_list[samity_id]->GetSamity_week_day(schedule_date)<<" schedule_date "<<schedule_date<<"last_date "<<last_date<<" Disburse_date()"<<ln->GetDisburse_date()<<"\n";
                ////exit(1); 
                if (reschedule_list.empty())
                    while (schedule_date < last_date) {

                        ln->addNewOneTimeFLATSchedule_date(schedule_date);
                        count += 1;
                        week_count++;
                        //schedule_date = (*samity_schedules_index)[week_count];
                        this->get_weekly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());

                    } else
                    while (schedule_date < last_date) {
                        ln->addNewOneTimeFLATSchedule_date(schedule_date);
                        count += 1;
                        week_count++;
                        //schedule_date = (*samity_schedules_index)[week_count];
                        this->get_weekly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());


                        for (it = reschedule_list.begin(); it != reschedule_list.end(); it++) {
                            if (schedule_date >= it->start && schedule_date <= it->end) {
                                schedule_date = it->end;
                                //schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                                //week_count = (*samity_schedules)[schedule_date];
                                this->get_weekly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());

                                break;
                            }
                        }
                    }
                ln->addNewOneTimeSchedule_date(schedule_date);
            }
        } else if (ln->GetInterest_calculation_method().compare("F") == 0) {

            if (ln->GetRepayment_frequency().compare("D") == 0) {
                while (count < no_of_ins) {
                    ln->addNewFLATSchedule_date(schedule_date);
                    is_rescheduled = false;
                    count += 1;
                    this->get_daily_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId());
                }
                ln->addLastFLATSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("W") == 0) {


                //this->get_weekly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId());
                //std::cerr<<"First Installment Date "<<schedule_date<<std::endl;
                ////exit(1);
                while (count < no_of_ins) {

                    ln->addNewFLATSchedule_date(schedule_date);
                    count += 1;
                    this->get_weekly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId());
                }
                ln->addLastFLATSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("M") == 0 && ln->GetRepayment_day_or_week().compare("NA") != 0) {
                //schedule_date=schedule_date+boost::gregorian::months(1);
                //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
                boost::gregorian::date first_schedule_date = schedule_date;
                //this->get_monthly_valid_schedule_date_consider_date(first_schedule_date, ln->GetSamity_id(), ln->GetId(), count,ln->GetRepayment_day(),ln->GetRepayment_week());
                if (ln->service_charge_taken_time == "EFS") {
                    while (count < no_of_ins) {

                        ln->addNewMonthlyPricipleFirstSchedule_date(schedule_date);

                        is_rescheduled = false;

                        schedule_date = first_schedule_date;
                        this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                        count += 1;
                    }
                    ln->addNewMonthlyPricipleLastSchedule_date(schedule_date);
                } else {
                    while (count < no_of_ins) {

                        ln->addNewFLATSchedule_date(schedule_date);

                        is_rescheduled = false;

                        schedule_date = first_schedule_date;
                        this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                        count += 1;
                    }
                    ln->addLastFLATSchedule_date(schedule_date);
                }
            } else if (ln->GetRepayment_frequency().compare("M") == 0) {
                boost::gregorian::date first_schedule_date = schedule_date;
                while (count < no_of_ins) {
                    ln->addNewFLATSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId(), count);
                    count += 1;
                }
                ln->addLastFLATSchedule_date(schedule_date);
            }
            else if (ln->GetRepayment_frequency().compare("2") == 0) {
                boost::gregorian::date first_schedule_date = schedule_date;
                while (count < no_of_ins) {
                    ln->addNewFLATSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId(), 2 * count);
                    count += 1;

                }
                ln->addLastFLATSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("3") == 0) {
                boost::gregorian::date first_schedule_date = schedule_date;
                while (count < no_of_ins) {
                    ln->addNewFLATSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId(), 3 * count);
                    count += 1;
                }
                ln->addLastFLATSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("4") == 0) {
                boost::gregorian::date first_schedule_date = schedule_date;
                while (count < no_of_ins) {
                    ln->addNewFLATSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId(), 4 * count);
                    count += 1;
                }
                ln->addLastFLATSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("5") == 0) {
                boost::gregorian::date first_schedule_date = schedule_date;
                while (count < no_of_ins) {
                    ln->addNewFLATSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId(), 5 * count);
                    count += 1;
                }
                ln->addLastFLATSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("6") == 0) {
                boost::gregorian::date first_schedule_date = schedule_date;
                while (count < no_of_ins) {
                    ln->addNewFLATSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId(), 6 * count);
                    count += 1;
                }
                ln->addLastFLATSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("Y") == 0) {
                while (count < no_of_ins) {
                    ln->addNewFLATSchedule_date(schedule_date);
                    is_rescheduled = false;
                    count += 1;
                    this->get_yearly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId());
                }
                ln->addLastFLATSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("O") == 0) {
                ln->addNewOneTimeSchedule_date(schedule_date);
            }

        } else {
            no_of_ins = ln->actual_number_of_installments;
            if (ln->GetRepayment_frequency().compare("D") == 0) {
                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date(schedule_date);
                    is_rescheduled = false;
                    count += 1;
                    this->get_daily_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId());
                }
                ln->addLastDECLININGSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("W") == 0) {
                //this->get_weekly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId());
                //int week_count = (*samity_schedules)[schedule_date];            
                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date(schedule_date);
                    count += 1;
                    //week_count++;
                    this->get_weekly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId());

                }
                ln->addLastDECLININGSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("M") == 0 && ln->GetRepayment_day_or_week().compare("NA") != 0) {
                //schedule_date=schedule_date+boost::gregorian::months(1);
                //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
                boost::gregorian::date first_schedule_date = schedule_date;
                //this->get_monthly_valid_schedule_date_consider_date(first_schedule_date, ln->GetSamity_id(), ln->GetId(), count,ln->GetRepayment_day(),ln->GetRepayment_week());
                if (ln->service_charge_taken_time == "EFS") {
                    while (count < no_of_ins) {

                        ln->addNewMonthlyPricipleFirstSchedule_date(schedule_date);

                        is_rescheduled = false;

                        schedule_date = first_schedule_date;
                        this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                        count += 1;
                    }
                    ln->addNewMonthlyPricipleLastSchedule_date(schedule_date);
                } else {
                    while (count < no_of_ins) {

                        ln->addNewDECLININGSchedule_date(schedule_date);

                        is_rescheduled = false;

                        schedule_date = first_schedule_date;
                        this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                        count += 1;
                    }
                    ln->addLastDECLININGSchedule_date(schedule_date);
                }
            } else if (ln->GetRepayment_frequency().compare("M") == 0) {
                boost::gregorian::date first_schedule_date = schedule_date;
                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId(), count);
                    count += 1;
                }
                ln->addLastDECLININGSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("2") == 0) {
                boost::gregorian::date first_schedule_date = schedule_date;
                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId(), 2 * count);
                    count += 1;
                }
                ln->addLastDECLININGSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("3") == 0) {
                boost::gregorian::date first_schedule_date = schedule_date;
                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId(), 3 * count);
                    count += 1;
                }
                ln->addLastDECLININGSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("4") == 0) {
                boost::gregorian::date first_schedule_date = schedule_date;
                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId(), 4 * count);
                    count += 1;
                }
                ln->addLastDECLININGSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("5") == 0) {
                boost::gregorian::date first_schedule_date = schedule_date;
                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId(), 5 * count);
                    count += 1;
                }
                ln->addLastDECLININGSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("6") == 0) {
                boost::gregorian::date first_schedule_date = schedule_date;
                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId(), 6 * count);
                    count += 1;
                }
                ln->addLastDECLININGSchedule_date(schedule_date);
            } else if (ln->GetRepayment_frequency().compare("Y") == 0) {
                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date(schedule_date);
                    is_rescheduled = false;
                    count += 1;
                    this->get_yearly_valid_schedule_date_consider_transfer(schedule_date, ln->GetSamity_id_consider_transfer(schedule_date), ln->GetId());
                }
            }
        }
        ln->SetLast_repayment_date(schedule_date);

        ln->SetTotal_payable_amount();
        ln->SetInstallment_amount();

    }



};

#endif /* LOAN_SCHEDULER_H */

