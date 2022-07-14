/*
 * File:   Loan_scheduler.cpp
 * Author: Murad
 *
 * Created on February 6, 2011, 12:48 PM
 */

#include <boost/detail/container_fwd.hpp>
//#include <boost/thread/thread.hpp>
#include "php.h"

#include "Loan_scheduler.h"

#include <sstream>
#include <iostream>
#include <boost/date_time/gregorian/parsers.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>

double _round(double d) {

    return d;
    //std::cerr<<d<<"\n";
    //d=lround(lround(d*1000)/10)/100;
    //d=(lround(d*100)/100);
    d = (round(d * 100) / 100);
    //std::cerr<<d<<"\n";

    char buffer[100];
    sprintf(buffer, "%.2f", d);
    d = strtod(buffer, NULL);
    ////exit(1);
    return d;
}

double _round10(double d) {

    //std::cerr<<d<<"\n";
    //d=lround(lround(d*1000)/10)/100;
    //d=(lround(d*100)/100);

    long factor = 10000000000;
    if (fabs(d * factor) < 1000) {
        return 0;
    }
    d = (round(d * factor) / factor);
    //std::cerr<<d<<"\n";

    char buffer[100];
    sprintf(buffer, "%.10f", d);
    d = strtod(buffer, NULL);

    ////exit(1);
    return d;
}

Loan_scheduler::Loan_scheduler() {
    std::ofstream fout;
    fout.open("/var/www/microfin360/trunk/application/logs/microfin360/scheduler.log", std::ios::out);
    fout << "Scheduler Created\n";

    branch_holiday_list_p = NULL;
}

Loan_scheduler::Loan_scheduler(const Loan_scheduler& orig) {
    std::ofstream fout;
    fout.open("/var/www/microfin360/trunk/application/logs/microfin360/scheduler.log", std::ios::out);
    fout << "Copy Scheduler Created\n";
}

Loan_scheduler::~Loan_scheduler() {

    std::ofstream fout;
    fout.open(" ", std::ios::app);
    fout << "Scheduler Destroyed\n";

    if (branch_holiday_list_p) {
        branch_holiday_list_p->clear();
        delete( branch_holiday_list_p);
    }
    branch_holiday_list.clear();
    samity_holiday_list.clear();
    samity_day_change_list.clear();

    for (__gnu_cxx::hash_map<int, samity*>::iterator it = samity_list.begin(); it != samity_list.end(); it++)
        delete(it->second);
    samity_list.clear();

    for (__gnu_cxx::hash_map<int, loan*>::iterator it = loan_list.begin(); it != loan_list.end(); it++)
        delete(it->second);
    loan_list.clear();

    branch_samity_loan_product_list.clear();
    branch_samity_member_loan_list.clear();
    branch_loan_list.clear();
    branch_samity_loan_list.clear();

    loan_reschedule_list.clear();
    delete(this->system_date);

}

bool Loan_scheduler::initialize(int branch_id, int samity_id, std::string loan_ids) {
    
}

void Loan_scheduler::post_initialize(long unsigned int start, long unsigned int end) {
    
    for (long unsigned int index = start; index < end; index++) {
        
        this->run_scheduler(this->loan_list[index]);

        branch_samity_loan_product_list[this->loan_list[index]->GetBranch_id()][this->loan_list[index]->GetSamity_id()][this->loan_list[index]->GetProduct_id()][this->loan_list[index]->GetId()] = (this->loan_list[index]);

        branch_samity_member_loan_list[this->loan_list[index]->GetBranch_id()][this->loan_list[index]->GetSamity_id()][this->loan_list[index]->GetMember_id()][this->loan_list[index]->GetId()] = (this->loan_list[index]);
        branch_loan_list[this->loan_list[index]->GetBranch_id()][this->loan_list[index]->GetId()] = (this->loan_list[index]);
        branch_samity_loan_list[this->loan_list[index]->GetBranch_id()][this->loan_list[index]->GetSamity_id()][this->loan_list[index]->GetId()] = (this->loan_list[index]);
    }

}

bool Loan_scheduler::initialize(boost::gregorian::date *system_date, int branch_id, int samity_id, std::string s_date_from, std::string s_date_to, br_hash* b_holiday, samity_date_hash samity_holiday_date_hash, /*std::vector<loan>u_loans,*/reschedule_hash_type loan_reschedule_list, samity_hash u_samities, loan_hash loan_list, int org_flag = 0) {
//    std::cerr << " In Initialize\n"; exit(1);
    this->system_date = system_date;
    this->org_flag = org_flag;
    boost::gregorian::date date_from = boost::gregorian::from_simple_string(s_date_from);
    boost::gregorian::date date_to = boost::gregorian::from_simple_string(s_date_to);

    this->samity_list = u_samities;

    this->branch_id = branch_id;
    this->samity_id = samity_id;
    this->date_from = date_from;
    this->date_to = date_to;
    this->loan_reschedule_list = loan_reschedule_list;
//    std::cerr << "before_branch_holiday" << std::endl;
    
    //this->branch_holiday_list = b_holiday;
    this->branch_holiday_list_p = b_holiday;
    
    this->samity_holiday_list = samity_holiday_date_hash;

    this->loan_list = loan_list;
  
    for (loan_hash::iterator it = this->loan_list.begin(); it != this->loan_list.end(); it++) {
       
        //std::cerr << "run_scheduler\n";
        this->run_scheduler(this->loan_list[it->first]);


        branch_samity_loan_product_list[this->loan_list[it->first]->GetBranch_id()][this->loan_list[it->first]->GetSamity_id()][this->loan_list[it->first]->GetProduct_id()][this->loan_list[it->first]->GetId()] = (this->loan_list[it->first]);

        branch_samity_member_loan_list[this->loan_list[it->first]->GetBranch_id()][this->loan_list[it->first]->GetSamity_id()][this->loan_list[it->first]->GetMember_id()][this->loan_list[it->first]->GetId()] = (this->loan_list[it->first]);
        branch_loan_list[this->loan_list[it->first]->GetBranch_id()][this->loan_list[it->first]->GetId()] = (this->loan_list[it->first]);
        branch_samity_loan_list[this->loan_list[it->first]->GetBranch_id()][this->loan_list[it->first]->GetSamity_id()][this->loan_list[it->first]->GetId()] = (this->loan_list[it->first]);
    }
    
    return true;
}

bool Loan_scheduler::reinitialize(reschedule_hash_type loan_reschedule_list) {
    this->loan_reschedule_list = loan_reschedule_list;
    for (loan_hash::iterator it = this->loan_list.begin(); it != this->loan_list.end(); it++) {
        this->run_scheduler(this->loan_list[it->first]);        
    }
}

std::string Loan_scheduler::get_first_schedule_date(int loan_id) {
    return boost::gregorian::to_iso_extended_string(loan_list[loan_id]->GetFirst_repayment_date());
}

std::string Loan_scheduler::get_last_schedule_date(int loan_id) {
    //std::cerr << "loan : " << loan_id << std::endl;
    return boost::gregorian::to_iso_extended_string(loan_list[loan_id]->GetLast_repayment_date());
}

std::string Loan_scheduler::get_schedule_date_by_installment_no(int loan_id, int installment_no) {
    return boost::gregorian::to_iso_extended_string(loan_list[loan_id]->schedule_datum[installment_no].schedule_date);
}

zval Loan_scheduler::get_schedule_installment_info_by_installment_no(int loan_id, int installment_no) {

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    add_assoc_double(&array1, "principle_installment_amount", loan_list[loan_id]->schedule_datum[installment_no].principle_installment_amount);
    add_assoc_double(&array1, "interrest_installment_amount", loan_list[loan_id]->schedule_datum[installment_no].interrest_installment_amount);
    add_assoc_double(&array1, "installment_amount", loan_list[loan_id]->schedule_datum[installment_no].installment_amount);
    add_assoc_string(&array1, "schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(loan_list[loan_id]->schedule_datum[installment_no].schedule_date).c_str()));

    return array1;
}

zval Loan_scheduler::get_loan_transaction_amount(int loan_id, std::string s_date) {

    //std::cerr << "In loan transactions\n";
    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);
    for (loan_hash::iterator it = loan_list.begin(); it != loan_list.end(); it++)
        //std::cerr << it->second->GetId() << std::endl;
        //std::cerr << loan_list[loan_id]->GetId() <<" "<<s_date << std::endl;

        if (!s_date.empty()) {
            boost::gregorian::date d_date = boost::gregorian::from_simple_string(s_date);
            int temp_i = 0;
            int index_flag = -1;
            for (temp_i = loan_list[loan_id]->getFirstInstallmentNumber(); temp_i < loan_list[loan_id]->getLastInstallmentNumber(); temp_i++) {
                if (loan_list[loan_id]->schedule_datum[temp_i].schedule_date > d_date) {
                    temp_i--;
                    break;
                }
                index_flag = temp_i;
            }
            temp_i = index_flag;
            if (temp_i != -1) {
                add_assoc_double(&array1, "principal_installment_amount", loan_list[loan_id]->schedule_datum[temp_i].principle_installment_amount);
                add_assoc_double(&array1, "principle_installment_amount", loan_list[loan_id]->schedule_datum[temp_i].principle_installment_amount);

                add_assoc_double(&array1, "interest_installment_amount", loan_list[loan_id]->schedule_datum[temp_i].interrest_installment_amount);
                add_assoc_double(&array1, "installment_no", temp_i + 1);
                add_assoc_string(&array1, "schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(loan_list[loan_id]->schedule_datum[temp_i].schedule_date).c_str()));
                return array1;
            }
        }

    add_assoc_double(&array1, "principal_installment_amount", loan_list[loan_id]->schedule_datum[loan_list[loan_id]->getFirstInstallmentNumber()].principle_installment_amount);
    add_assoc_double(&array1, "principle_installment_amount", loan_list[loan_id]->schedule_datum[loan_list[loan_id]->getFirstInstallmentNumber()].principle_installment_amount);

    add_assoc_double(&array1, "interest_installment_amount", loan_list[loan_id]->schedule_datum[loan_list[loan_id]->getFirstInstallmentNumber()].interrest_installment_amount);
    add_assoc_double(&array1, "installment_no", loan_list[loan_id]->getFirstInstallmentNumber());
    add_assoc_string(&array1, "schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(loan_list[loan_id]->schedule_datum[loan_list[loan_id]->getFirstInstallmentNumber()].schedule_date).c_str()));

    return array1;

}

zval Loan_scheduler::get_loan_transaction_amount_before_date(int loan_id, std::string s_date) {


    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    //std::cerr << "get_loan_transaction_amount_before_date " << loan_id << std::endl;

    if (!s_date.empty()) {
        boost::gregorian::date d_date = boost::gregorian::from_simple_string(s_date);

        zval subarray;
//        MAKE_STD_ZVAL(subarray);
        array_init(&subarray);

        int temp_i = 0;
        int flag = -1;
        temp_i = loan_list[loan_id]->getFirstInstallmentNumber();
        //std::cerr << loan_id << " " << temp_i + 1 << " " << loan_list[loan_id]->schedule_datum[temp_i].principle_installment_amount << " " << loan_list[loan_id]->schedule_datum[temp_i].interrest_installment_amount << std::endl;
        //std::cerr <<"Last "<<loan_list[loan_id]->getLastInstallmentNumber()<<std::endl;
        //for (temp_i = loan_list[loan_id]->getFirstInstallmentNumber(); temp_i < loan_list[loan_id]->getLastInstallmentNumber(); temp_i++) {
        for (temp_i = 0; temp_i < loan_list[loan_id]->getLastInstallmentNumber(); temp_i++) {

            if (loan_list[loan_id]->schedule_datum[temp_i].schedule_date < d_date) {
                flag = 1;
                zval subarray1;
//                MAKE_STD_ZVAL(subarray1);
                array_init(&subarray1);
                //std::cerr << loan_id << " " << temp_i + 1 << " " << loan_list[loan_id]->schedule_datum[temp_i].principle_installment_amount << " " << loan_list[loan_id]->schedule_datum[temp_i].interrest_installment_amount << std::endl;

                add_assoc_double(&subarray1, "installment_amount", loan_list[loan_id]->schedule_datum[temp_i].installment_amount);
                add_assoc_double(&subarray1, "principal_installment_amount", loan_list[loan_id]->schedule_datum[temp_i].principle_installment_amount);
                add_assoc_double(&subarray1, "principle_installment_amount", loan_list[loan_id]->schedule_datum[temp_i].principle_installment_amount);

                add_assoc_double(&subarray1, "interest_installment_amount", loan_list[loan_id]->schedule_datum[temp_i].interrest_installment_amount);
                add_assoc_double(&subarray1, "installment_no", temp_i + 1);
                add_index_zval(&subarray, temp_i + 1, &subarray1);

            }
        }
        //std::cerr << " Murad Outside\n";
        if (flag == 1) {
            //std::cerr << " After Murad Inside\n";
            add_index_zval(&array1, loan_id, &subarray);
        }
        //std::cerr << " After Murad Outside\n";
    }

    return array1;

}

zval Loan_scheduler::get_loan_recoverable_amount(int branch_id, int product_id, std::string date) {

    zval subarray;
//    MAKE_STD_ZVAL(subarray);
    array_init(&subarray);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<int, float> samity_recov;
    //samity_recov samity_recovery;
    boost::gregorian::date d_date = boost::gregorian::from_simple_string(date);

    for (samity_loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id].begin(); it != branch_samity_loan_product_list[branch_id].end(); it++) {
        double recov = 0;

        for (loan_hash::iterator it1 = it->second[product_id].begin(); it1 != it->second[product_id].end(); it1++) {
            if (it1->second->GetLoan_closing_date() > d_date)
                for (int i = it1->second->getFirstInstallmentNumber(); i < it1->second->getLastInstallmentNumber(); i++) {
                    if (it1->second->schedule_datum[i].schedule_date == d_date) {
                        recov = recov + it1->second->schedule_datum[i].installment_amount;
                        break;
                    }
                }


        }
        //samity_recovery[it->first] = recov;
        add_index_double(&subarray, it->first, recov);
    }

    return subarray;
}


zval Loan_scheduler::get_loan_recoverable_amount_branch_id(int branch_id, std::string s_date_frm, std::string s_date_to) {


    //std::cerr<<"get_loan_recoverable_amount_branch_id "<<branch_id<<" "<<s_date_frm<<" "<<s_date_to<<std::endl;

    zval subarray;
//    MAKE_STD_ZVAL(subarray);
    array_init(&subarray);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<std::string, schedule_data> schedule_datum_hash_map;

    typedef __gnu_cxx::hash_map<int, double> product_recov;
    product_recov product_recov_male_p;
    product_recov product_recov_female_p;
    product_recov product_recov_male_i;
    product_recov product_recov_female_i;
    //std::cerr << branch_id << " " << s_date_frm << " " << s_date_to << std::endl;
    if (!s_date_frm.empty() && !s_date_to.empty()) {
        boost::gregorian::date d_date_from = boost::gregorian::from_simple_string(s_date_frm);
        boost::gregorian::date d_date_to = boost::gregorian::from_simple_string(s_date_to);
        //std::cerr<<"Dates "<<d_date_from<<" "<<d_date_to<<std::endl;
        for (samity_loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id].begin(); it != branch_samity_loan_product_list[branch_id].end(); it++) {

            for (loan_product_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
                double recov_m_p = 0;
                double recov_f_p = 0;
                double recov_m_i = 0;
                double recov_f_i = 0;

                if (product_recov_male_p.find(it1->first) == product_recov_male_p.end()) {
                    product_recov_male_p[it1->first] = 0;
                }
                if (product_recov_female_p.find(it1->first) == product_recov_female_p.end()) {
                    product_recov_female_p[it1->first] = 0;
                }
                if (product_recov_male_i.find(it1->first) == product_recov_male_i.end()) {
                    product_recov_male_i[it1->first] = 0;
                }
                if (product_recov_female_i.find(it1->first) == product_recov_female_i.end()) {
                    product_recov_female_i[it1->first] = 0;
                }

                for (loan_hash::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
                    for (int it3 = it2->second->getFirstInstallmentNumber(); it3 < it2->second->getLastInstallmentNumber(); it3++) {
                        //for (schedule_datum_hash_map::iterator it3 = it2->second->schedule_datum_hash.begin(); it3 != it2->second->schedule_datum_hash.end(); ++it3) {
                        //if ((boost::gregorian::from_simple_string(it3->first) > d_date_from) && (boost::gregorian::from_simple_string(it3->first) <= d_date_to)) {
                        if ((it2->second->GetDisburse_date() > d_date_from) && (it2->second->GetDisburse_date() <= d_date_to)) {
                            if ((samity_list[it->first]->GetSamity_type().compare("M") == 0) || (samity_list[it->first]->GetSamity_type().compare("m") == 0)) {
                                recov_m_p = recov_m_p + it2->second->schedule_datum[it3].principle_installment_amount; //it3->second.principle_installment_amount;
                                recov_m_i = recov_m_i + it2->second->schedule_datum[it3].interrest_installment_amount; //it3->second.interrest_installment_amount;
                            } else {
                                recov_f_p = recov_f_p + it2->second->schedule_datum[it3].principle_installment_amount; //it3->second.principle_installment_amount;
                                recov_f_i = recov_f_i + it2->second->schedule_datum[it3].interrest_installment_amount; //it3->second.interrest_installment_amount;
                            }
                        }
                    }

                }
                product_recov_male_p[it1->first] = product_recov_male_p[it1->first] + recov_m_p;
                product_recov_male_i[it1->first] = product_recov_male_i[it1->first] + recov_m_i;
                product_recov_female_p[it1->first] = product_recov_female_p[it1->first] + recov_f_p;
                product_recov_female_i[it1->first] = product_recov_female_i[it1->first] + recov_f_i;

                zval subarray1;
//                MAKE_STD_ZVAL(subarray1);
                array_init(&subarray1);

                zval subarray2;
//                MAKE_STD_ZVAL(subarray2);
                array_init(&subarray2);

                zval subarray3;
//                MAKE_STD_ZVAL(subarray3);
                array_init(&subarray3);

                add_assoc_double(&subarray2, "principle_installment_amount", product_recov_male_p[it1->first]);
                add_assoc_double(&subarray2, "interrest_installment_amount", product_recov_male_i[it1->first]);
                add_assoc_zval(&subarray1, "M", &subarray2);

                add_assoc_double(&subarray3, "principle_installment_amount", product_recov_female_p[it1->first]);
                add_assoc_double(&subarray3, "interrest_installment_amount", product_recov_female_i[it1->first]);
                add_assoc_zval(&subarray1, "F", &subarray3);

                add_index_zval(&subarray, it1->first, &subarray1);
            }
            //std::cerr<<"Samity_id "<<it->first<<std::endl;
        }
    } else if (!s_date_frm.empty()) {
        boost::gregorian::date d_date_from = boost::gregorian::from_simple_string(s_date_frm);
        //boost::gregorian::date d_date_to=boost::gregorian::from_simple_string(s_date_to);
        for (samity_loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id].begin(); it != branch_samity_loan_product_list[branch_id].end(); it++) {

            for (loan_product_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
                double recov_m_p = 0;
                double recov_f_p = 0;
                double recov_m_i = 0;
                double recov_f_i = 0;

                if (product_recov_male_p.find(it1->first) == product_recov_male_p.end()) {
                    product_recov_male_p[it1->first] = 0;
                }
                if (product_recov_female_p.find(it1->first) == product_recov_female_p.end()) {
                    product_recov_female_p[it1->first] = 0;
                }
                if (product_recov_male_i.find(it1->first) == product_recov_male_i.end()) {
                    product_recov_male_i[it1->first] = 0;
                }
                if (product_recov_female_i.find(it1->first) == product_recov_female_i.end()) {
                    product_recov_female_i[it1->first] = 0;
                }

                for (loan_hash::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
                    for (int it3 = it2->second->getFirstInstallmentNumber(); it3 < it2->second->getLastInstallmentNumber(); it3++) {
                        //for (schedule_datum_hash_map::iterator it3 = it2->second->schedule_datum_hash.begin(); it3 != it2->second->schedule_datum_hash.end(); ++it3) {
                        //if (boost::gregorian::from_simple_string(it3->first) < d_date_from) {
                        if (it2->second->GetDisburse_date() <= d_date_from) {
                            if ((samity_list[it->first]->GetSamity_type().compare("M") == 0) || (samity_list[it->first]->GetSamity_type().compare("m") == 0)) {
                                recov_m_p = recov_m_p + it2->second->schedule_datum[it3].principle_installment_amount; //it3->second.principle_installment_amount;
                                recov_m_i = recov_m_i + it2->second->schedule_datum[it3].interrest_installment_amount; //it3->second.interrest_installment_amount;
                            } else {
                                recov_f_p = recov_f_p + it2->second->schedule_datum[it3].principle_installment_amount; //it3->second.principle_installment_amount;
                                recov_f_i = recov_f_i + it2->second->schedule_datum[it3].interrest_installment_amount; //it3->second.interrest_installment_amount;
                            }
                        }
                    }



                }
                product_recov_male_p[it1->first] = product_recov_male_p[it1->first] + recov_m_p;
                product_recov_male_i[it1->first] = product_recov_male_i[it1->first] + recov_m_i;
                product_recov_female_p[it1->first] = product_recov_female_p[it1->first] + recov_f_p;
                product_recov_female_i[it1->first] = product_recov_female_i[it1->first] + recov_f_i;

                zval subarray1;
//                MAKE_STD_ZVAL(subarray1);
                array_init(&subarray1);

                zval subarray2;
//                MAKE_STD_ZVAL(subarray2);
                array_init(&subarray2);

                zval subarray3;
//                MAKE_STD_ZVAL(subarray3);
                array_init(&subarray3);

                add_assoc_double(&subarray2, "principle_installment_amount", product_recov_male_p[it1->first]);
                add_assoc_double(&subarray2, "interrest_installment_amount", product_recov_male_i[it1->first]);
                add_assoc_zval(&subarray1, "M", &subarray2);

                add_assoc_double(&subarray3, "principle_installment_amount", product_recov_female_p[it1->first]);
                add_assoc_double(&subarray3, "interrest_installment_amount", product_recov_female_i[it1->first]);
                add_assoc_zval(&subarray1, "F", &subarray3);

                add_index_zval(&subarray, it1->first, &subarray1);
            }
            //samity_recovery[it->first]=recov;
            //add_index_double(subarray,it->first,recov);
        }
    }

    return subarray;
}

zval Loan_scheduler::get_branch_wise_loan_recoverable_amount_by_product_id(int branch_id, std::string s_date_frm, std::string s_date_to) {
    zval subarray;
//    MAKE_STD_ZVAL(subarray);
    array_init(&subarray);


    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<std::string, schedule_data> schedule_datum_hash_map;

    typedef __gnu_cxx::hash_map<int, double> product_recov;
    product_recov product_recov_male_p;
    product_recov product_recov_female_p;
    product_recov product_recov_male_i;
    product_recov product_recov_female_i;

    if (!s_date_frm.empty() && !s_date_to.empty()) {
        boost::gregorian::date d_date_from = boost::gregorian::from_simple_string(s_date_frm);
        boost::gregorian::date d_date_to = boost::gregorian::from_simple_string(s_date_to);
        for (samity_loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id].begin(); it != branch_samity_loan_product_list[branch_id].end(); it++) {

            for (loan_product_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
                double recov_m_p = 0;
                double recov_f_p = 0;
                double recov_m_i = 0;
                double recov_f_i = 0;

                if (product_recov_male_p.find(it1->first) == product_recov_male_p.end()) {
                    product_recov_male_p[it1->first] = 0;
                }
                if (product_recov_female_p.find(it1->first) == product_recov_female_p.end()) {
                    product_recov_female_p[it1->first] = 0;
                }
                if (product_recov_male_i.find(it1->first) == product_recov_male_i.end()) {
                    product_recov_male_i[it1->first] = 0;
                }
                if (product_recov_female_i.find(it1->first) == product_recov_female_i.end()) {
                    product_recov_female_i[it1->first] = 0;
                }

                for (loan_hash::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
                    for (int it3 = it2->second->getFirstInstallmentNumber(); it3 < it2->second->getLastInstallmentNumber(); it3++) {
                        //for (schedule_datum_hash_map::iterator it3 = it2->second->schedule_datum_hash.begin(); it3 != it2->second->schedule_datum_hash.end(); ++it3) {
                        if ((it2->second->schedule_datum[it3].schedule_date > d_date_from) && (it2->second->schedule_datum[it3].schedule_date <= d_date_to)) {
                            //if ((it2->second->GetDisburse_date() > d_date_from) && (it2->second->GetDisburse_date() <= d_date_to)){
                            if ((samity_list[it->first]->GetSamity_type().compare("M") == 0) || (samity_list[it->first]->GetSamity_type().compare("m") == 0)) {
                                recov_m_p = recov_m_p + it2->second->schedule_datum[it3].principle_installment_amount; //it3->second.principle_installment_amount;
                                recov_m_i = recov_m_i + it2->second->schedule_datum[it3].interrest_installment_amount; //it3->second.interrest_installment_amount;
                            } else {
                                recov_f_p = recov_f_p + it2->second->schedule_datum[it3].principle_installment_amount; //it3->second.principle_installment_amount;
                                recov_f_i = recov_f_i + it2->second->schedule_datum[it3].interrest_installment_amount; //it3->second.interrest_installment_amount;
                            }
                        }
                    }

                }
                product_recov_male_p[it1->first] = product_recov_male_p[it1->first] + recov_m_p;
                product_recov_male_i[it1->first] = product_recov_male_i[it1->first] + recov_m_i;
                product_recov_female_p[it1->first] = product_recov_female_p[it1->first] + recov_f_p;
                product_recov_female_i[it1->first] = product_recov_female_i[it1->first] + recov_f_i;

                zval subarray1;
//                MAKE_STD_ZVAL(subarray1);
                array_init(&subarray1);

                zval subarray2;
//                MAKE_STD_ZVAL(subarray2);
                array_init(&subarray2);

                zval subarray3;
//                MAKE_STD_ZVAL(subarray3);
                array_init(&subarray3);

                add_assoc_double(&subarray2, "principle_installment_amount", product_recov_male_p[it1->first]);
                add_assoc_double(&subarray2, "interrest_installment_amount", product_recov_male_i[it1->first]);
                add_assoc_zval(&subarray1, "M", &subarray2);

                add_assoc_double(&subarray3, "principle_installment_amount", product_recov_female_p[it1->first]);
                add_assoc_double(&subarray3, "interrest_installment_amount", product_recov_female_i[it1->first]);
                add_assoc_zval(&subarray1, "F", &subarray3);

                add_index_zval(&subarray, it1->first, &subarray1);
            }
            //samity_recovery[it->first]=recov;
            //add_index_double(subarray,it->first,recov);
        }
    } else if (!s_date_frm.empty()) {
        boost::gregorian::date d_date_from = boost::gregorian::from_simple_string(s_date_frm);
        //boost::gregorian::date d_date_to=boost::gregorian::from_simple_string(s_date_to);
        for (samity_loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id].begin(); it != branch_samity_loan_product_list[branch_id].end(); it++) {

            for (loan_product_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
                double recov_m_p = 0;
                double recov_f_p = 0;
                double recov_m_i = 0;
                double recov_f_i = 0;

                if (product_recov_male_p.find(it1->first) == product_recov_male_p.end()) {
                    product_recov_male_p[it1->first] = 0;
                }
                if (product_recov_female_p.find(it1->first) == product_recov_female_p.end()) {
                    product_recov_female_p[it1->first] = 0;
                }
                if (product_recov_male_i.find(it1->first) == product_recov_male_i.end()) {
                    product_recov_male_i[it1->first] = 0;
                }
                if (product_recov_female_i.find(it1->first) == product_recov_female_i.end()) {
                    product_recov_female_i[it1->first] = 0;
                }

                for (loan_hash::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
                    for (int it3 = it2->second->getFirstInstallmentNumber(); it3 < it2->second->getFirstInstallmentNumber(); it3++) {
                        //for (schedule_datum_hash_map::iterator it3 = it2->second->schedule_datum_hash.begin(); it3 != it2->second->schedule_datum_hash.end(); ++it3) {
                        if (it2->second->schedule_datum[it3].schedule_date < d_date_from) {
                            //if (it2->second->GetDisburse_date() <= d_date_from) {
                            if ((samity_list[it->first]->GetSamity_type().compare("M") == 0) || (samity_list[it->first]->GetSamity_type().compare("m") == 0)) {
                                recov_m_p = recov_m_p + it2->second->schedule_datum[it3].principle_installment_amount; //it3->second.principle_installment_amount;
                                recov_m_i = recov_m_i + it2->second->schedule_datum[it3].interrest_installment_amount; //it3->second.interrest_installment_amount;
                            } else {
                                recov_f_p = recov_f_p + it2->second->schedule_datum[it3].principle_installment_amount; //it3->second.principle_installment_amount;
                                recov_f_i = recov_f_i + it2->second->schedule_datum[it3].interrest_installment_amount; //it3->second.interrest_installment_amount;
                            }
                        }
                    }



                }
                product_recov_male_p[it1->first] = product_recov_male_p[it1->first] + recov_m_p;
                product_recov_male_i[it1->first] = product_recov_male_i[it1->first] + recov_m_i;
                product_recov_female_p[it1->first] = product_recov_female_p[it1->first] + recov_f_p;
                product_recov_female_i[it1->first] = product_recov_female_i[it1->first] + recov_f_i;

                zval subarray1;
//                MAKE_STD_ZVAL(subarray1);
                array_init(&subarray1);

                zval subarray2;
//                MAKE_STD_ZVAL(subarray2);
                array_init(&subarray2);

                zval subarray3;
//                MAKE_STD_ZVAL(subarray3);
                array_init(&subarray3);

                add_assoc_double(&subarray2, "principle_installment_amount", product_recov_male_p[it1->first]);
                add_assoc_double(&subarray2, "interrest_installment_amount", product_recov_male_i[it1->first]);
                add_assoc_zval(&subarray1, "M", &subarray2);

                add_assoc_double(&subarray3, "principle_installment_amount", product_recov_female_p[it1->first]);
                add_assoc_double(&subarray3, "interrest_installment_amount", product_recov_female_i[it1->first]);
                add_assoc_zval(&subarray1, "F", &subarray3);

                add_index_zval(&subarray, it1->first, &subarray1);
            }
            //samity_recovery[it->first]=recov;
            //add_index_double(subarray,it->first,recov);
        }
    }

    return subarray;
}

zval Loan_scheduler::get_branch_wise_loan_recoverable_amount(int branch_id, std::string s_date_frm, std::string s_date_to) {

    //std::cerr << "In get_branch_wise_loan_recoverable_amount\n";
    //std::cerr << branch_id << " " << s_date_frm << " " << s_date_to << std::endl;
    zval subarray;
//    MAKE_STD_ZVAL(subarray);
    array_init(&subarray);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<std::string, schedule_data> schedule_datum_hash_map;

    typedef __gnu_cxx::hash_map<int, float> samity_recov;
    samity_recov samity_recovery;


    double recov_m_p = 0;
    double recov_f_p = 0;
    double recov_m_i = 0;
    double recov_f_i = 0;

    double recov = 0;
    double recov_ins = 0;


    if (!s_date_frm.empty() && !s_date_to.empty()) {
        boost::gregorian::date d_date_from = boost::gregorian::from_simple_string(s_date_frm);
        boost::gregorian::date d_date_to = boost::gregorian::from_simple_string(s_date_to);
        for (samity_loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id].begin(); it != branch_samity_loan_product_list[branch_id].end(); it++) {
            //std::cerr << "In First Loop" << std::endl;
            for (loan_product_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
                //std::cerr << "Second Loop" << std::endl;

                for (loan_hash::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
                    //std::cerr << "In Third Loop" << std::endl;
                    //for (schedule_datum_hash_map::iterator it3 = it2->second->schedule_datum_hash.begin(); it3 != it2->second->schedule_datum_hash.end(); ++it3) {
                    for (int it3 = it2->second->getFirstInstallmentNumber(); it3 < it2->second->getLastInstallmentNumber(); it3++) {
                        //std::cerr << "In Fourth Loop" << std::endl;
                        if ((it2->second->GetFirst_repayment_date() > d_date_from) && (it2->second->GetFirst_repayment_date() <= d_date_to)) {

                            recov_ins = recov_ins + it2->second->schedule_datum[it3].installment_amount;

                        }

                        if ((it2->second->schedule_datum[it3].schedule_date > d_date_from) && (it2->second->schedule_datum[it3].schedule_date <= d_date_to)) {

                            recov = recov + it2->second->schedule_datum[it3].installment_amount;

                        }
                    }

                }

            }
            //samity_recovery[it->first]=recov;
            //add_index_double(subarray,it->first,recov);
        }
    } else if (!s_date_frm.empty()) {
        //std::cerr << s_date_frm << std::endl;
        boost::gregorian::date d_date_from = boost::gregorian::from_simple_string(s_date_frm);
        //boost::gregorian::date d_date_to=boost::gregorian::from_simple_string(s_date_to);
        for (samity_loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id].begin(); it != branch_samity_loan_product_list[branch_id].end(); it++) {

            for (loan_product_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {


                for (loan_hash::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
                    //                    double recov_m_p = 0;
                    //                    double recov_f_p = 0;
                    //                    double recov_m_i = 0;
                    //                    double recov_f_i = 0;
                    for (int it3 = 0; it3 < it2->second->schedule_datum.size(); it3++) {
                        //for (schedule_datum_hash_map::iterator it3 = it2->second->schedule_datum_hash.begin(); it3 != it2->second->schedule_datum_hash.end(); ++it3) {
                        if (it2->second->GetFirst_repayment_date() <= d_date_from) {
                            recov_ins = recov_ins + it2->second->schedule_datum[it3].installment_amount;                           
                        }

                        if (it2->second->schedule_datum[it3].schedule_date <= d_date_from) {
                            recov = recov + it2->second->schedule_datum[it3].installment_amount;                           
                        }
                    }

                }

            }
            //samity_recovery[it->first]=recov;
            //add_index_double(subarray,it->first,recov);
        }
    }


    zval subarray1;
//    MAKE_STD_ZVAL(subarray1);
    array_init(&subarray1);


    add_assoc_double(&subarray1, "projected_recovery", recov);
    add_assoc_double(&subarray1, "recovarable", recov_ins);


    add_index_zval(&subarray, branch_id, &subarray1);

    return subarray;
}

zval Loan_scheduler::get_branch_wise_loan_recoverable_amount_consider_fully_paid(int branch_id, std::string s_date_frm, std::string s_date_to) {


    //std::cerr << branch_id << " " << s_date_frm << " " << s_date_to << std::endl;
    zval subarray;
//    MAKE_STD_ZVAL(subarray);
    array_init(&subarray);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<std::string, schedule_data> schedule_datum_hash_map;

    typedef __gnu_cxx::hash_map<int, float> samity_recov;
    samity_recov samity_recovery;


    double recov_m_p = 0;
    double recov_f_p = 0;
    double recov_m_i = 0;
    double recov_f_i = 0;

    double recov = 0;
    double recov_ins = 0;


    if (!s_date_frm.empty() && !s_date_to.empty()) {
        boost::gregorian::date d_date_from = boost::gregorian::from_simple_string(s_date_frm);
        boost::gregorian::date d_date_to = boost::gregorian::from_simple_string(s_date_to);
        for (samity_loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id].begin(); it != branch_samity_loan_product_list[branch_id].end(); it++) {
            //std::cerr << "In First Loop" << std::endl;
            for (loan_product_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
                //std::cerr << "Second Loop" << std::endl;

                for (loan_hash::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
                    //std::cerr << "In Third Loop" << std::endl;
                    //for (schedule_datum_hash_map::iterator it3 = it2->second->schedule_datum_hash.begin(); it3 != it2->second->schedule_datum_hash.end(); ++it3) {
                    for (int it3 = it2->second->getFirstInstallmentNumber(); it3 < it2->second->getLastInstallmentNumber(); it3++) {
                        //std::cerr << "In Fourth Loop" << std::endl;
                        if ((it2->second->GetFirst_repayment_date() > d_date_from) && (it2->second->GetFirst_repayment_date() <= d_date_to)) {

                            recov_ins = recov_ins + it2->second->schedule_datum[it3].installment_amount;

                        }

                        if ((it2->second->schedule_datum[it3].schedule_date > d_date_from) && (it2->second->schedule_datum[it3].schedule_date <= d_date_to) && it2->second->GetLoan_fully_paid_date() > d_date_to) {

                            recov = recov + it2->second->schedule_datum[it3].installment_amount;

                        }
                    }

                }

            }
            //samity_recovery[it->first]=recov;
            //add_index_double(subarray,it->first,recov);
        }
    } else if (!s_date_frm.empty()) {
        boost::gregorian::date d_date_from = boost::gregorian::from_simple_string(s_date_frm);
        //boost::gregorian::date d_date_to=boost::gregorian::from_simple_string(s_date_to);
        for (samity_loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id].begin(); it != branch_samity_loan_product_list[branch_id].end(); it++) {

            for (loan_product_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {


                for (loan_hash::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
                    //                    double recov_m_p = 0;
                    //                    double recov_f_p = 0;
                    //                    double recov_m_i = 0;
                    //                    double recov_f_i = 0;
                    for (int it3 = 0; it3 < it2->second->schedule_datum.size(); it3++) {
                        //for (schedule_datum_hash_map::iterator it3 = it2->second->schedule_datum_hash.begin(); it3 != it2->second->schedule_datum_hash.end(); ++it3) {
                        if (it2->second->GetFirst_repayment_date() <= d_date_from) {

                            recov_ins = recov_ins + it2->second->schedule_datum[it3].installment_amount;

                        }

                        if (it2->second->schedule_datum[it3].schedule_date <= d_date_from && it2->second->GetLoan_fully_paid_date() > d_date_from) {

                            recov = recov + it2->second->schedule_datum[it3].installment_amount;


                        }
                    }

                }

            }
            //samity_recovery[it->first]=recov;
            //add_index_double(subarray,it->first,recov);
        }
    }


    zval subarray1;
//    MAKE_STD_ZVAL(subarray1);
    array_init(&subarray1);


    add_assoc_double(&subarray1, "projected_recovery", recov);
    add_assoc_double(&subarray1, "recovarable", recov_ins);


    add_index_zval(&subarray, branch_id, &subarray1);

    return subarray;
}

zval Loan_scheduler::get_loan_installment_information_by_loan_id_trasaction_amount(int loan_id, double total_transaction_amount) {

    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;
    int index;
    double sum = 0;

    zval subarray1;
//    MAKE_STD_ZVAL(subarray1);
    array_init(&subarray1);

    for (index = this->loan_list[loan_id]->getFirstInstallmentNumber(); index<this->loan_list[loan_id]->getLastInstallmentNumber(); index++) {
        sum = sum + this->loan_list[loan_id]->schedule_datum[index].installment_amount;
        if (sum > total_transaction_amount) {


            zval subarray2;
//            MAKE_STD_ZVAL(subarray2);
            array_init(&subarray2);
            add_assoc_double(&subarray2, "installment_amount", this->loan_list[loan_id]->schedule_datum[index].installment_amount);
            add_assoc_double(&subarray2, "principle_installment_amount", this->loan_list[loan_id]->schedule_datum[index].principle_installment_amount);
            add_assoc_double(&subarray2, "interest_installment_amount", this->loan_list[loan_id]->schedule_datum[index].interrest_installment_amount);
            add_assoc_long(&subarray2, "installment_number", index + 1);
            add_assoc_string(&subarray2, "schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(this->loan_list[loan_id]->schedule_datum[index].schedule_date).c_str()));

            add_index_zval(&subarray1, index + 1, &subarray2);
        }
    }

    add_assoc_double(&subarray1, "total_recoverable_amount", this->loan_list[loan_id]->GetTotal_payable_amount() - total_transaction_amount);

    return subarray1;
}


////////////////////

zval Loan_scheduler::get_loan_schedules_by_memeberlist_branch_product(std::vector<int> member_list, int branch_id, int product_id, int samity_id, std::string s_date_frm, std::string s_date_to) {

    //std::cerr<<"get_loan_schedules_by_memeberlist_branch_product"<<std::endl;
    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<int, float> samity_recov;
    samity_recov samity_recovery;

    if (samity_id && !s_date_frm.empty() && !s_date_to.empty()) {
        boost::gregorian::date d_date_frm(boost::gregorian::from_simple_string(s_date_frm));
        boost::gregorian::date d_date_to(boost::gregorian::from_simple_string(s_date_to));
        for (int i = 0; i < member_list.size(); i++) {
            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            for (loan_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_id][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_id][product_id].end(); it++) {

                if ((it->second->GetMember_id() == member_list[i]) && (d_date_frm > it->second->GetDisburse_date()) && (it->second->GetLoan_closing_date() > d_date_frm)) {
                    zval subarray1;
//                    MAKE_STD_ZVAL(subarray1);
                    array_init(&subarray1);

                    double recoverable_amount = 0;
                    for (int index = it->second->getFirstInstallmentNumber(); index < it->second->getLastInstallmentNumber(); index++) {
                        if (it->second->schedule_datum[index].schedule_date > d_date_frm && it->second->schedule_datum[index].schedule_date < d_date_to) {
                            zval subarray2;
//                            MAKE_STD_ZVAL(subarray2);
                            array_init(&subarray2);

                            recoverable_amount = recoverable_amount + it->second->schedule_datum[index].installment_amount;

                            add_assoc_double(&subarray2, "installment_amount", it->second->schedule_datum[index].installment_amount);
                            add_assoc_double(&subarray2, "principle_installment_amount", it->second->schedule_datum[index].principle_installment_amount);
                            add_assoc_double(&subarray2, "interest_installment_amount", it->second->schedule_datum[index].interrest_installment_amount);
                            add_assoc_long(&subarray2, "installment_number", index + 1);
                            add_assoc_string(&subarray2, "schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[index].schedule_date).c_str()));
                            //add_assoc_string(subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                            std::string temp_last_date;
                            temp_last_date = this->get_last_schedule_date(it->second->GetId());
                            boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                            add_assoc_string(&subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                            add_index_zval(&subarray1, index + 1, &subarray2);
                        }
                    }
                    //add_index_zval(subarray,it->first,subarray2);
                    add_assoc_double(&subarray1, "recoverable_amount", recoverable_amount);
                    add_index_zval(&subarray, it->first, &subarray1);
                }
            }
            add_index_zval(&array1, member_list[i], &subarray);
        }
    } else if (samity_id) {
        //std::cout<<branch_id<<" "<<samity_id<<" "<<product_id<<std::endl;
        ////exit(1);
        for (int i = 0; i < member_list.size(); i++) {
            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            for (loan_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_id][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_id][product_id].end(); it++) {
                //std::cout<<it->second.GetMember_id()<<" "<<member_list[i]<<std::endl;
                if ((it->second->GetMember_id() == member_list[i])) {
                    //std::cout<<samity_id<<std::endl;
                    zval subarray1;
//                    MAKE_STD_ZVAL(subarray1);
                    array_init(&subarray1);
                    double recoverable_amount = 0;
                    for (int index = it->second->getFirstInstallmentNumber(); index < it->second->getLastInstallmentNumber(); index++) {
                        //if(it->second.schedule_datum[index].schedule_date>d_date_frm && it->second.schedule_datum[index].schedule_date<d_date_to){
                        zval subarray2;
//                        MAKE_STD_ZVAL(subarray2);
                        array_init(&subarray2);

                        recoverable_amount = recoverable_amount + it->second->schedule_datum[index].installment_amount;

                        add_assoc_double(&subarray2, "installment_amount", it->second->schedule_datum[index].installment_amount);
                        add_assoc_double(&subarray2, "principle_installment_amount", it->second->schedule_datum[index].principle_installment_amount);
                        add_assoc_double(&subarray2, "interest_installment_amount", it->second->schedule_datum[index].interrest_installment_amount);
                        add_assoc_long(&subarray2, "installment_number", index + 1);
                        add_assoc_string(&subarray2, "schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[index].schedule_date).c_str()));
                        //add_assoc_string(subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                        std::string temp_last_date;
                        temp_last_date = this->get_last_schedule_date(it->second->GetId());
                        boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                        add_assoc_string(&subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                        add_index_zval(&subarray1, index + 1, &subarray2);
                        //}
                    }
                    add_assoc_double(&subarray1, "recoverable_amount", recoverable_amount);
                    add_index_zval(&subarray, it->first, &subarray1);
                }
            }
            add_index_zval(&array1, member_list[i], &subarray);

        }
    } else {
        for (samity_loan_product_hash::iterator itt = branch_samity_loan_product_list[branch_id].begin(); itt != branch_samity_loan_product_list[branch_id].end(); itt++) {
            for (loan_hash::iterator it = itt->second[product_id].begin(); it != itt->second[product_id].end(); it++) {
                for (int i = 0; i < member_list.size(); i++) {
                    //std::cerr<<"Member ID "<<it->second->GetMember_id()<<" Member ID "<<member_list[i]<<std::endl;
                    if (it->second->GetMember_id() == member_list[i]) {
                        zval subarray;
//                        MAKE_STD_ZVAL(subarray);
                        array_init(&subarray);
                        double recoverable_amount = 0;
                        for (int index = it->second->getFirstInstallmentNumber(); index < it->second->getLastInstallmentNumber(); index++) {
                            //if(it->second.schedule_datum[index].schedule_date>d_date_frm && it->second.schedule_datum[index].schedule_date<d_date_to){
                            //std::cerr<< it->second->GetId()<<std::endl;
                            zval subarray1;
//                            MAKE_STD_ZVAL(subarray1);
                            array_init(&subarray1);

                            recoverable_amount = recoverable_amount + it->second->schedule_datum[index].installment_amount;

                            add_assoc_double(&subarray1, "installment_amount", it->second->schedule_datum[index].installment_amount);
                            add_assoc_double(&subarray1, "principle_installment_amount", it->second->schedule_datum[index].principle_installment_amount);
                            add_assoc_double(&subarray1, "interest_installment_amount", it->second->schedule_datum[index].interrest_installment_amount);
                            add_assoc_long(&subarray1, "installment_number", index + 1);
                            add_assoc_string(&subarray1, "schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[index].schedule_date).c_str()));
                            //add_assoc_string(subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                            std::string temp_last_date;
                            temp_last_date = this->get_last_schedule_date(it->second->GetId());
                            boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                            add_assoc_string(&subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                            add_index_zval(&subarray, index + 1, &subarray1);
                            //}
                        }
                        //add_assoc_double(subarray, "recoverable_amount", recoverable_amount);
                        //zval *subarray2;
                        //MAKE_STD_ZVAL(subarray2);
                        //array_init(subarray2);
                        //add_index_zval(subarray2,  it->second->GetId(), subarray);
                        add_index_zval(&array1, member_list[i], &subarray);

                    }
                }

            }
        }

    }
    return array1;
}

////////////////////

zval Loan_scheduler::get_member_wise_loan_schedules_by_branch(std::vector<int> member_list, int branch_id, std::string s_date_frm, std::string s_date_to) {



    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<int, float> samity_recov;
    samity_recov samity_recovery;

    if (!s_date_frm.empty() && !s_date_to.empty()) {
        boost::gregorian::date d_date_frm(boost::gregorian::from_simple_string(s_date_frm));
        boost::gregorian::date d_date_to(boost::gregorian::from_simple_string(s_date_to));
        for (int i = 0; i < member_list.size(); i++) {
            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);
            bool sub_array_flag = false;
            for (samity_loan_product_hash::iterator itt1 = branch_samity_loan_product_list[branch_id].begin(); itt1 != branch_samity_loan_product_list[branch_id].end(); itt1++) {
                for (loan_product_hash::iterator itt2 = itt1->second.begin(); itt2 != itt1->second.end(); itt2++) {
                    for (loan_hash::iterator it = itt2->second.begin(); it != itt2->second.end(); it++) {

                        if ((it->second->GetMember_id() == member_list[i]) && (d_date_to > it->second->GetDisburse_date()) && (it->second->GetCurrent_status() == 1)) {
                            zval subarray1;
//                            MAKE_STD_ZVAL(subarray1);
                            array_init(&subarray1);
                            bool recoverable_amount_flag = false;
                            double recoverable_amount = 0;
                            for (int index = it->second->getFirstInstallmentNumber(); index < it->second->getLastInstallmentNumber(); index++) {
                                if (it->second->schedule_datum[index].schedule_date > d_date_frm && it->second->schedule_datum[index].schedule_date <= d_date_to) {
                                    zval subarray2;
//                                    MAKE_STD_ZVAL(subarray2);
                                    array_init(&subarray2);

                                    recoverable_amount_flag = true;

                                    recoverable_amount = recoverable_amount + it->second->schedule_datum[index].installment_amount;

                                    add_assoc_double(&subarray2, "installment_amount", it->second->schedule_datum[index].installment_amount);
                                    add_assoc_double(&subarray2, "principle_installment_amount", it->second->schedule_datum[index].principle_installment_amount);
                                    add_assoc_double(&subarray2, "interest_installment_amount", it->second->schedule_datum[index].interrest_installment_amount);
                                    add_assoc_long(&subarray2, "installment_number", index + 1);
                                    add_assoc_string(&subarray2, "schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[index].schedule_date).c_str()));
                                    //add_assoc_string(subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                                    std::string temp_last_date;
                                    temp_last_date = this->get_last_schedule_date(it->second->GetId());
                                    boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                                    add_assoc_string(&subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                                    add_index_zval(&subarray1, index + 1, &subarray2);
                                }
                            }
                            //add_index_zval(subarray,it->first,subarray2);
                            if (recoverable_amount_flag == true) {
                                add_assoc_double(&subarray1, "recoverable_amount", recoverable_amount);
                                add_index_zval(&subarray, it->first, &subarray1);
                                sub_array_flag = true;
                            }
                        }
                    }
                }
            }
            if (sub_array_flag == true) {
                add_index_zval(&array1, member_list[i], &subarray);
            }
        }
    } else if (!s_date_frm.empty()) {
        boost::gregorian::date d_date_frm(boost::gregorian::from_simple_string(s_date_frm));
        //boost::gregorian::date d_date_to(boost::gregorian::from_simple_string(s_date_to));
        for (int i = 0; i < member_list.size(); i++) {
            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);
            bool sub_array_flag = false;
            for (samity_loan_product_hash::iterator itt1 = branch_samity_loan_product_list[branch_id].begin(); itt1 != branch_samity_loan_product_list[branch_id].end(); itt1++) {
                for (loan_product_hash::iterator itt2 = itt1->second.begin(); itt2 != itt1->second.end(); itt2++) {
                    for (loan_hash::iterator it = itt2->second.begin(); it != itt2->second.end(); it++) {

                        if ((it->second->GetMember_id() == member_list[i]) && (d_date_frm > it->second->GetDisburse_date()) && (it->second->GetCurrent_status() == 1)) {
                            zval subarray1;
//                            MAKE_STD_ZVAL(subarray1);
                            array_init(&subarray1);
                            bool recoverable_amount_flag = false;
                            double recoverable_amount = 0;
                            for (int index = it->second->getFirstInstallmentNumber(); index < it->second->getLastInstallmentNumber(); index++) {
                                if (it->second->schedule_datum[index].schedule_date <= d_date_frm) {
                                    zval subarray2;
//                                    MAKE_STD_ZVAL(subarray2);
                                    array_init(&subarray2);

                                    recoverable_amount = recoverable_amount + it->second->schedule_datum[index].installment_amount;

                                    add_assoc_double(&subarray2, "installment_amount", it->second->schedule_datum[index].installment_amount);
                                    add_assoc_double(&subarray2, "principle_installment_amount", it->second->schedule_datum[index].principle_installment_amount);
                                    add_assoc_double(&subarray2, "interest_installment_amount", it->second->schedule_datum[index].interrest_installment_amount);
                                    add_assoc_long(&subarray2, "installment_number", index + 1);
                                    add_assoc_string(&subarray2, "schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[index].schedule_date).c_str()));
                                    //add_assoc_string(subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                                    std::string temp_last_date;
                                    temp_last_date = this->get_last_schedule_date(it->second->GetId());
                                    boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                                    add_assoc_string(&subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                                    add_index_zval(&subarray1, index + 1, &subarray2);
                                }
                            }
                            //add_index_zval(subarray,it->first,subarray2);
                            if (recoverable_amount_flag == true) {
                                add_assoc_double(&subarray1, "recoverable_amount", recoverable_amount);
                                add_index_zval(&subarray, it->first, &subarray1);
                                sub_array_flag = true;
                            }
                        }
                    }
                }
            }
            if (sub_array_flag == true) {
                add_index_zval(&array1, member_list[i], &subarray);
            }
        }
    } else {
        boost::gregorian::date d_date_frm(boost::gregorian::from_simple_string(s_date_frm));
        boost::gregorian::date d_date_to(boost::gregorian::from_simple_string(s_date_to));
        for (int i = 0; i < member_list.size(); i++) {
            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            bool sub_array_flag = false;

            for (samity_loan_product_hash::iterator itt1 = branch_samity_loan_product_list[branch_id].begin(); itt1 != branch_samity_loan_product_list[branch_id].end(); itt1++) {
                for (loan_product_hash::iterator itt2 = itt1->second.begin(); itt2 != itt1->second.end(); itt2++) {
                    for (loan_hash::iterator it = itt2->second.begin(); it != itt2->second.end(); it++) {

                        //if ((it->second->GetMember_id() == member_list[i]) && (d_date_frm > it->second->GetDisburse_date()) && (it->second->GetCurrent_status() == 1)) {
                        zval subarray1;
//                        MAKE_STD_ZVAL(subarray1);
                        array_init(&subarray1);
                        bool recoverable_amount_flag = false;
                        double recoverable_amount = 0;
                        for (int index = it->second->getFirstInstallmentNumber(); index < it->second->getLastInstallmentNumber(); index++) {
                            if (it->second->schedule_datum[index].schedule_date <= d_date_frm) {
                                zval subarray2;
//                                MAKE_STD_ZVAL(subarray2);
                                array_init(&subarray2);

                                recoverable_amount = recoverable_amount + it->second->schedule_datum[index].installment_amount;

                                add_assoc_double(&subarray2, "installment_amount", it->second->schedule_datum[index].installment_amount);
                                add_assoc_double(&subarray2, "principle_installment_amount", it->second->schedule_datum[index].principle_installment_amount);
                                add_assoc_double(&subarray2, "interest_installment_amount", it->second->schedule_datum[index].interrest_installment_amount);
                                add_assoc_long(&subarray2, "installment_number", index + 1);
                                add_assoc_string(&subarray2, "schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[index].schedule_date).c_str()));
                                //add_assoc_string(subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                                std::string temp_last_date;
                                temp_last_date = this->get_last_schedule_date(it->second->GetId());
                                boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                                add_assoc_string(&subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                                add_index_zval(&subarray1, index + 1, &subarray2);
                            }
                        }
                        //add_index_zval(subarray,it->first,subarray2);
                        if (recoverable_amount_flag == true) {
                            add_assoc_double(&subarray1, "recoverable_amount", recoverable_amount);
                            add_index_zval(&subarray, it->first, &subarray1);
                            sub_array_flag = true;
                        }
                        //}
                    }
                }
            }
            if (sub_array_flag == true) {
                add_index_zval(&array1, member_list[i], &subarray);
            }
        }
    }
    return array1;
}

zval Loan_scheduler::get_loan_schedules_samity_wise(int branch_id, int samity_id) {
    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);
    if (branch_id && samity_id) {
        typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > member_loan_hash;
        typedef __gnu_cxx::hash_map<int, loan*> loan_hash;
        zval subarray;
//        MAKE_STD_ZVAL(subarray);
        array_init(&subarray);
        int count = 0;
        for (member_loan_hash::iterator it = branch_samity_member_loan_list[branch_id][samity_id].begin(); it != branch_samity_member_loan_list[branch_id][samity_id].end(); ++it) {
            //std::cerr << "counter " << ++count << std::endl;
            for (loan_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); ++it1) {

                for (int it2 = it1->second->getFirstInstallmentNumber(); it2 < it1->second->getLastInstallmentNumber(); it2++) {

                    zval subarray1;
//                    MAKE_STD_ZVAL(subarray1);
                    array_init(&subarray1);

                    add_assoc_long(&subarray1, "loan_id", it1->second->GetId());
                    add_assoc_long(&subarray1, "member_id", it1->second->GetMember_id());
                    add_assoc_long(&subarray1, "branch_id", it1->second->GetBranch_id());
                    add_assoc_long(&subarray1, "samity_id", it1->second->GetSamity_id());
                    add_assoc_long(&subarray1, "product_id", it1->second->GetProduct_id());
                    add_assoc_long(&subarray1, "installment_number", it2 + 1);
                    add_assoc_double(&subarray1, "installment_amount", it1->second->schedule_datum[it2].installment_amount);
                    add_assoc_double(&subarray1, "principal_installment_amount", it1->second->schedule_datum[it2].principle_installment_amount);
                    add_assoc_double(&subarray1, "principle_installment_amount", it1->second->schedule_datum[it2].principle_installment_amount);
                    add_assoc_double(&subarray1, "extra_installment_amount", it1->second->extra_amount);


                    add_assoc_double(&subarray1, "interest_installment_amount", it1->second->schedule_datum[it2].interrest_installment_amount);
                    add_assoc_string(&subarray1, "schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[it2].schedule_date).c_str()));

                    add_next_index_zval(&subarray, &subarray1);
                }
                add_next_index_zval(&array1, &subarray);
            }
        }
    }
    return array1;
}




///////////////////

zval Loan_scheduler::get_loan_schedules(int branch_id, int samity_id, int member_id, int loan_id) {

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    //std::cerr << "In Loan Schedules function" << std::endl;
    std::cerr << branch_id << " " << samity_id << " " << member_id << " " << loan_id << std::endl;

    if (branch_id && samity_id && member_id && loan_id) {

        zval subarray;
//        MAKE_STD_ZVAL(subarray);
        array_init(&subarray);
        //std::cerr << branch_id << " " << samity_id << " " << member_id << " " << loan_id << std::endl;
        //std::cerr<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->getFirstInstallmentNumber()<<" "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->getLastInstallmentNumber()<<"\n";

        for (int it = branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->getFirstInstallmentNumber(); it < branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->getLastInstallmentNumber(); it++) {
            //for (int it = 0; it < branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum.size(); it++) {

            zval subarray1;
//            MAKE_STD_ZVAL(subarray1);
            array_init(&subarray1);
            //double te=177.78;
            //add_assoc_double(subarray1, "te", te);
            //std::cerr << "In Loan Schedules loan_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetId() << std::endl;
            add_assoc_long(&subarray1, "loan_id", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetId());
            //std::cerr << "In Loan Schedules member_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetMember_id() << std::endl;

            add_assoc_long(&subarray1, "member_id", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetMember_id());
            //std::cerr << "In Loan Schedules branch_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetBranch_id() << std::endl;

            add_assoc_long(&subarray1, "branch_id", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetBranch_id());
            //std::cerr << "In Loan Schedules samity_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetSamity_id() << std::endl;
            add_assoc_long(&subarray1, "samity_id", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetSamity_id());
            //std::cerr << "In Loan Schedules product_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetProduct_id() << std::endl;
            add_assoc_long(&subarray1, "product_id", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetProduct_id());
            //std::cerr << "In Loan Schedules installment_number "<<it+1 << std::endl;
            add_assoc_long(&subarray1, "installment_number", it + 1);
            //std::cerr << "In Loan Schedules installment_amount "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].installment_amount << std::endl;
            add_assoc_double(&subarray1, "installment_amount", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].installment_amount);
            //std::cerr << "In Loan Schedules principal_installment_amount "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].principle_installment_amount << std::endl;
            add_assoc_double(&subarray1, "principal_installment_amount", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].principle_installment_amount);
            add_assoc_double(&subarray1, "principle_installment_amount", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].principle_installment_amount);
            add_assoc_double(&subarray1, "extra_installment_amount", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->extra_amount);

            //std::cerr << "In Loan Schedules interest_installment_amount "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].interrest_installment_amount << std::endl;
            add_assoc_double(&subarray1, "interest_installment_amount", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].interrest_installment_amount);
            //std::cerr << "In Loan Schedules schedule_date "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].schedule_date << std::endl;
            std::string tmp(boost::gregorian::to_iso_extended_string(branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].schedule_date).c_str());
            add_assoc_string(&subarray1, "schedule_date", const_cast<char*> (tmp.c_str()));
            //std::cerr << "Done With In Loan Schedules loan_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetId() << std::endl;
            add_next_index_zval(&subarray, &subarray1);
            //std::cout<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id].schedule_datum[it].installment_amount<<" "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id].schedule_datum[it].principle_installment_amount<<" "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id].schedule_datum[it].interrest_installment_amount<<std::endl;

        }
        //std::cerr << "Outside" << std::endl;
        add_next_index_zval(&array1, &subarray);
        //std::cerr << "Outside2" << std::endl;
    } else if (branch_id && samity_id && member_id) {
        typedef __gnu_cxx::hash_map<int, loan*> loan_hash;
        for (loan_hash::iterator it = branch_samity_member_loan_list[branch_id][samity_id][member_id].begin(); it != branch_samity_member_loan_list[branch_id][samity_id][member_id].end(); ++it) {

            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);
            for (int it1 = it->second->getFirstInstallmentNumber(); it1 < it->second->getLastInstallmentNumber(); it1++) {

                zval subarray1;
//                MAKE_STD_ZVAL(subarray1);
                array_init(&subarray1);

                add_assoc_long(&subarray1, "loan_id", it->second->GetId());
                add_assoc_long(&subarray1, "member_id", it->second->GetMember_id());
                add_assoc_long(&subarray1, "branch_id", it->second->GetBranch_id());
                add_assoc_long(&subarray1, "samity_id", it->second->GetSamity_id());
                add_assoc_long(&subarray1, "product_id", it->second->GetProduct_id());
                add_assoc_long(&subarray1, "installment_number", it1 + 1);
                add_assoc_double(&subarray1, "installment_amount", it->second->schedule_datum[it1].installment_amount);
                add_assoc_double(&subarray1, "principal_installment_amount", it->second->schedule_datum[it1].principle_installment_amount);
                add_assoc_double(&subarray1, "principle_installment_amount", it->second->schedule_datum[it1].principle_installment_amount);
                add_assoc_double(&subarray1, "extra_installment_amount", it->second->extra_amount);

                add_assoc_double(&subarray1, "interest_installment_amount", it->second->schedule_datum[it1].interrest_installment_amount);
                add_assoc_string(&subarray1, "schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it1].schedule_date).c_str()));

                add_next_index_zval(&subarray, &subarray1);
            }
            add_next_index_zval(&array1, &subarray);
        }
    } else if (branch_id && samity_id) {
        typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > member_loan_hash;
        typedef __gnu_cxx::hash_map<int, loan*> loan_hash;
        zval subarray;
//        MAKE_STD_ZVAL(subarray);
        array_init(&subarray);
        for (member_loan_hash::iterator it = branch_samity_member_loan_list[branch_id][samity_id].begin(); it != branch_samity_member_loan_list[branch_id][samity_id].end(); ++it) {

            for (loan_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); ++it1) {

                for (int it2 = it1->second->getFirstInstallmentNumber(); it2 < it1->second->getLastInstallmentNumber(); it2++) {

                    zval subarray1;
//                    MAKE_STD_ZVAL(subarray1);
                    array_init(&subarray1);

                    add_assoc_long(&subarray1, "loan_id", it1->second->GetId());
                    add_assoc_long(&subarray1, "member_id", it1->second->GetMember_id());
                    add_assoc_long(&subarray1, "branch_id", it1->second->GetBranch_id());
                    add_assoc_long(&subarray1, "samity_id", it1->second->GetSamity_id());
                    add_assoc_long(&subarray1, "product_id", it1->second->GetProduct_id());
                    add_assoc_long(&subarray1, "installment_number", it2 + 1);
                    add_assoc_double(&subarray1, "installment_amount", it1->second->schedule_datum[it2].installment_amount);
                    add_assoc_double(&subarray1, "principal_installment_amount", it1->second->schedule_datum[it2].principle_installment_amount);
                    add_assoc_double(&subarray1, "principle_installment_amount", it1->second->schedule_datum[it2].principle_installment_amount);
                    add_assoc_double(&subarray1, "extra_installment_amount", it1->second->extra_amount);


                    add_assoc_double(&subarray1, "interest_installment_amount", it1->second->schedule_datum[it2].interrest_installment_amount);
                    add_assoc_string(&subarray1, "schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[it2].schedule_date).c_str()));

                    add_next_index_zval(&subarray, &subarray1);
                }
                add_next_index_zval(&array1, &subarray);
            }
        }
    } else if (branch_id && loan_id) {
        branch_id = this->loan_list[loan_id]->GetBranch_id();
        samity_id = this->loan_list[loan_id]->GetSamity_id();
        member_id = this->loan_list[loan_id]->GetMember_id();
        zval subarray;
//        MAKE_STD_ZVAL(subarray);
        array_init(&subarray);
        //std::cerr << branch_id << " " << samity_id << " " << member_id << " " << loan_id << std::endl;
        //std::cerr<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->getFirstInstallmentNumber()<<" "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->getLastInstallmentNumber()<<"\n";

        for (int it = branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->getFirstInstallmentNumber(); it < branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->getLastInstallmentNumber(); it++) {
            //for (int it = 0; it < branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum.size(); it++) {

            zval subarray1;
//            MAKE_STD_ZVAL(subarray1);
            array_init(&subarray1);
            //double te=177.78;
            //add_assoc_double(subarray1, "te", te);
            //std::cerr << "In Loan Schedules loan_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetId() << std::endl;
            add_assoc_long(&subarray1, "loan_id", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetId());
            //std::cerr << "In Loan Schedules member_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetMember_id() << std::endl;

            add_assoc_long(&subarray1, "member_id", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetMember_id());
            //std::cerr << "In Loan Schedules branch_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetBranch_id() << std::endl;

            add_assoc_long(&subarray1, "branch_id", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetBranch_id());
            //std::cerr << "In Loan Schedules samity_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetSamity_id() << std::endl;
            add_assoc_long(&subarray1, "samity_id", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetSamity_id());
            //std::cerr << "In Loan Schedules product_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetProduct_id() << std::endl;
            add_assoc_long(&subarray1, "product_id", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetProduct_id());
            //std::cerr << "In Loan Schedules installment_number "<<it+1 << std::endl;
            add_assoc_long(&subarray1, "installment_number", it + 1);
            //std::cerr << "In Loan Schedules installment_amount "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].installment_amount << std::endl;
            add_assoc_double(&subarray1, "installment_amount", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].installment_amount);
            //std::cerr << "In Loan Schedules principal_installment_amount "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].principle_installment_amount << std::endl;
            add_assoc_double(&subarray1, "principal_installment_amount", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].principle_installment_amount);
            add_assoc_double(&subarray1, "principle_installment_amount", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].principle_installment_amount);
            add_assoc_double(&subarray1, "extra_installment_amount", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->extra_amount);

            //std::cerr << "In Loan Schedules interest_installment_amount "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].interrest_installment_amount << std::endl;
            add_assoc_double(&subarray1, "interest_installment_amount", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].interrest_installment_amount);
            //std::cerr << "In Loan Schedules schedule_date "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].schedule_date << std::endl;
            std::string tmp(boost::gregorian::to_iso_extended_string(branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].schedule_date).c_str());
            add_assoc_string(&subarray1, "schedule_date", const_cast<char*> (tmp.c_str()));
            //std::cerr << "Done With In Loan Schedules loan_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetId() << std::endl;
            add_next_index_zval(&subarray, &subarray1);
            //std::cout<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id].schedule_datum[it].installment_amount<<" "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id].schedule_datum[it].principle_installment_amount<<" "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id].schedule_datum[it].interrest_installment_amount<<std::endl;

        }
        //std::cerr << "Outside" << std::endl;
        add_next_index_zval(&array1, &subarray);
        //std::cerr << "Outside2" << std::endl;
    } else if (branch_id) {
        typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_member_loan_hash;
        typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > member_loan_hash;
        typedef __gnu_cxx::hash_map<int, loan*> loan_hash;
        zval subarray;
//        MAKE_STD_ZVAL(subarray);
        array_init(&subarray);
        for (samity_member_loan_hash::iterator it = branch_samity_member_loan_list[branch_id].begin(); it != branch_samity_member_loan_list[branch_id].end(); ++it) {

            for (member_loan_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); ++it1) {

                for (loan_hash::iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {

                    for (int it3 = it2->second->getFirstInstallmentNumber(); it3 < it2->second->getFirstInstallmentNumber(); it3++) {

                        zval subarray1;
//                        MAKE_STD_ZVAL(subarray1);
                        array_init(&subarray1);

                        add_assoc_long(&subarray1, "loan_id", it2->second->GetId());
                        add_assoc_long(&subarray1, "member_id", it2->second->GetMember_id());
                        add_assoc_long(&subarray1, "branch_id", it2->second->GetBranch_id());
                        add_assoc_long(&subarray1, "samity_id", it2->second->GetSamity_id());
                        add_assoc_long(&subarray1, "product_id", it2->second->GetProduct_id());
                        add_assoc_long(&subarray1, "installment_number", it3 + 1);
                        add_assoc_double(&subarray1, "installment_amount", it2->second->schedule_datum[it3].installment_amount);
                        add_assoc_double(&subarray1, "principal_installment_amount", it2->second->schedule_datum[it3].principle_installment_amount);
                        add_assoc_double(&subarray1, "principle_installment_amount", it2->second->schedule_datum[it3].principle_installment_amount);
                        add_assoc_double(&subarray1, "extra_installment_amount", it2->second->extra_amount);


                        add_assoc_double(&subarray1, "interest_installment_amount", it2->second->schedule_datum[it3].interrest_installment_amount);
                        add_assoc_string(&subarray1, "schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it3].schedule_date).c_str()));

                        add_next_index_zval(&subarray, &subarray1);
                    }
                    add_next_index_zval(&array1, &subarray);
                }
            }
        }
    }


    //std::cerr << "Before return" << std::endl;
    return array1;



}

zval Loan_scheduler::get_full_loan_schedules(int branch_id, int samity_id, int member_id, int loan_id) {

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    //std::cerr << "In Full Loan Schedules" << std::endl;
    //std::cerr << branch_id << " " << samity_id << " " << member_id << " " << loan_id << std::endl;

    if (branch_id && samity_id && member_id && loan_id) {

        zval subarray;
//        MAKE_STD_ZVAL(subarray);
        array_init(&subarray);


        //for (int it = branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->getFirstInstallmentNumber(); it < branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->getLastInstallmentNumber(); it++) {
        for (int it = 0; it < branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum.size(); it++) {

            zval subarray1;
//            MAKE_STD_ZVAL(subarray1);
            array_init(&subarray1);
            //double te=177.78;
            //add_assoc_double(subarray1, "te", te);
            //std::cerr << "In Loan Schedules loan_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetId() << std::endl;
            add_assoc_long(&subarray1, "loan_id", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetId());
            //std::cerr << "In Loan Schedules member_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetMember_id() << std::endl;

            add_assoc_long(&subarray1, "member_id", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetMember_id());
            //std::cerr << "In Loan Schedules branch_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetBranch_id() << std::endl;

            add_assoc_long(&subarray1, "branch_id", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetBranch_id());
            //std::cerr << "In Loan Schedules samity_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetSamity_id() << std::endl;
            add_assoc_long(&subarray1, "samity_id", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetSamity_id());
            //std::cerr << "In Loan Schedules product_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetProduct_id() << std::endl;
            add_assoc_long(&subarray1, "product_id", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetProduct_id());
            //std::cerr << "In Loan Schedules installment_number "<<it+1 << std::endl;
            add_assoc_long(&subarray1, "installment_number", it + 1);
            //std::cerr << "In Loan Schedules installment_amount "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].installment_amount << std::endl;
            add_assoc_double(&subarray1, "installment_amount", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].installment_amount);
            //std::cerr << "In Loan Schedules principal_installment_amount "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].principle_installment_amount << std::endl;
            add_assoc_double(&subarray1, "principal_installment_amount", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].principle_installment_amount);

            //std::cerr << "In Loan Schedules interest_installment_amount "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].interrest_installment_amount << std::endl;
            add_assoc_double(&subarray1, "interest_installment_amount", branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].interrest_installment_amount);
            //std::cerr << "In Loan Schedules schedule_date "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].schedule_date << std::endl;
            std::string tmp(boost::gregorian::to_iso_extended_string(branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->schedule_datum[it].schedule_date).c_str());
            add_assoc_string(&subarray1, "schedule_date", const_cast<char*> (tmp.c_str()));
            //std::cerr << "Done With In Loan Schedules loan_id "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id]->GetId() << std::endl;
            add_next_index_zval(&subarray, &subarray1);
            //std::cout<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id].schedule_datum[it].installment_amount<<" "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id].schedule_datum[it].principle_installment_amount<<" "<<branch_samity_member_loan_list[branch_id][samity_id][member_id][loan_id].schedule_datum[it].interrest_installment_amount<<std::endl;

        }
        //std::cerr << "Outside" << std::endl;
        add_next_index_zval(&array1, &subarray);
        //std::cerr << "Outside2" << std::endl;
    }
    return array1;
}

zval Loan_scheduler::get_active_member_loan_schedule_info(std::vector<int> member_list, std::string s_date, int branch_id, int product_id, int samity_id) {

    boost::gregorian::date d_date(boost::gregorian::from_simple_string(s_date));

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<int, float> samity_recov;
    samity_recov samity_recovery;

    if (samity_id) {

        for (int i = 0; i < member_list.size(); i++) {
            zval subarray1;
//            MAKE_STD_ZVAL(subarray1);
            array_init(&subarray1);

            for (loan_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_id][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_id][product_id].end(); it++) {

                if ((it->second->GetMember_id() == member_list[i]) && (d_date > it->second->GetDisburse_date()) && (it->second->GetLoan_fully_paid_date() > d_date)) {
                    zval subarray;
//                    MAKE_STD_ZVAL(subarray);
                    array_init(&subarray);

                    add_assoc_double(&subarray, "installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].installment_amount);
                    add_assoc_double(&subarray, "principle_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].principle_installment_amount);
                    add_assoc_double(&subarray, "interest_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].interrest_installment_amount);
                    add_assoc_long(&subarray, "installment_number", it->second->GetActualNumber_of_installment());
                    //add_assoc_string(subarray, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                    std::string temp_last_date;
                    temp_last_date = this->get_last_schedule_date(it->second->GetId());
                    boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                    add_assoc_string(&subarray, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                    add_index_zval(&subarray1, it->second->GetId(), &subarray);

                }
            }
            add_index_zval(&array1, member_list[i], &subarray1);
        }
    } else {
        for (int i = 0; i < member_list.size(); i++) {
            zval subarray1;
//            MAKE_STD_ZVAL(subarray1);
            array_init(&subarray1);

            for (samity_loan_product_hash::iterator itt = branch_samity_loan_product_list[branch_id].begin(); itt != branch_samity_loan_product_list[branch_id].end(); itt++) {
                for (loan_hash::iterator it = itt->second[product_id].begin(); it != itt->second[product_id].end(); it++) {

                    if ((it->second->GetMember_id() == member_list[i]) && (d_date > it->second->GetDisburse_date()) && (it->second->GetLoan_fully_paid_date() > d_date)) {
                        zval subarray;
//                        MAKE_STD_ZVAL(subarray);
                        array_init(&subarray);

                        add_assoc_double(&subarray, "installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].installment_amount);
                        add_assoc_double(&subarray, "principle_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].principle_installment_amount);
                        add_assoc_double(&subarray, "interest_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].interrest_installment_amount);

                        add_assoc_long(&subarray, "installment_number", it->second->GetActualNumber_of_installment());
                        //add_assoc_string(subarray, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                        std::string temp_last_date;
                        temp_last_date = this->get_last_schedule_date(it->second->GetId());
                        boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                        add_assoc_string(&subarray, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                        //add_index_zval(array1, member_list[i], subarray);
                        add_index_zval(&subarray1, it->second->GetId(), &subarray);

                    }

                }

            }
            add_index_zval(&array1, member_list[i], &subarray1);
        }

    }
    return array1;
}
////////////////////////////////////////////////////////////////////////

/*
zval* Loan_scheduler::get_member_wise_current_loan_information_just_before_date(std::vector<int> member_list, std::string s_date_frm, int branch_id, int product_id, int samity_id, std::string s_date_to) {

    //std::cerr << "get_member_wise_current_loan_information_just_before_date\n";

    zval *array1;
    MAKE_STD_ZVAL(array1);
    array_init(array1);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<int, float> samity_recov;
    samity_recov samity_recovery;

    if (samity_id && s_date_to.empty()) {
        boost::gregorian::date d_date(boost::gregorian::from_simple_string(s_date_frm));
        for (loan_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_id][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_id][product_id].end(); it++) {
            for (int i = 0; i < member_list.size(); i++) {
                if ((it->second->GetMember_id() == member_list[i]) && (d_date > it->second->GetDisburse_date()) && (it->second->GetLoan_fully_paid_date() > d_date)) {
                    int temp_i = -1;
                    int over_due_flag = -1;
                    for (temp_i = it->second->getFirstInstallmentNumber(); temp_i < it->second->getLastInstallmentNumber(); temp_i++) {
                        //std::cerr << it->second->schedule_datum[temp_i].schedule_date << std::endl;
                        if (it->second->schedule_datum[temp_i].schedule_date < d_date) {
                            if (over_due_flag == -1) {
                                over_due_flag = temp_i;
                            }
                        }
                        if (it->second->schedule_datum[temp_i].schedule_date >= d_date) {

                            temp_i--;
                            break;
                        }
                    }
                    //std::cerr << "After Date Loop" << std::endl;
                    if (temp_i < 0 || temp_i == it->second->schedule_datum.size()) {
                        if (over_due_flag == -1)
                            continue;
                        temp_i = over_due_flag;
                    }

                    zval *subarray;
                    MAKE_STD_ZVAL(subarray);
                    array_init(subarray);

                    add_assoc_double(subarray, "installment_amount", it->second->schedule_datum[temp_i].installment_amount);
                    add_assoc_double(subarray, "principle_installment_amount", it->second->schedule_datum[temp_i].principle_installment_amount);
                    add_assoc_double(subarray, "interest_installment_amount", it->second->schedule_datum[temp_i].interrest_installment_amount);
                    add_assoc_long(subarray, "installment_number", temp_i + 1);

                    add_assoc_string(subarray, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                    add_assoc_string(subarray, "first_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->getFirstInstallmentNumber()].schedule_date).c_str()), 1);
                    add_assoc_long(subarray, "last_installment_number", it->second->GetActualNumber_of_installment());
                    add_assoc_string(subarray, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[temp_i].schedule_date).c_str()), 1);


                    add_index_zval(array1, member_list[i], subarray);

                }
            }

        }
    } else if (s_date_to.empty()) {
        boost::gregorian::date d_date(boost::gregorian::from_simple_string(s_date_frm));
        for (samity_loan_product_hash::iterator itt = branch_samity_loan_product_list[branch_id].begin(); itt != branch_samity_loan_product_list[branch_id].end(); itt++) {
            for (loan_hash::iterator it = itt->second[product_id].begin(); it != itt->second[product_id].end(); it++) {
                for (int i = 0; i < member_list.size(); i++) {
                    if ((it->second->GetMember_id() == member_list[i]) && (d_date > it->second->GetDisburse_date()) && (it->second->GetLoan_fully_paid_date() > d_date)) {

                        int temp_i = -1;
                        int over_due_flag = -1;
                        for (temp_i = it->second->getFirstInstallmentNumber(); temp_i < it->second->getLastInstallmentNumber(); temp_i++) {
                            //std::cerr << it->second->schedule_datum[temp_i].schedule_date << std::endl;
                            if (it->second->schedule_datum[temp_i].schedule_date < d_date) {
                                if (over_due_flag == -1) {
                                    over_due_flag = temp_i;
                                }
                            }
                            if (it->second->schedule_datum[temp_i].schedule_date >= d_date) {
                                temp_i--;
                                break;
                            }
                        }
                        //std::cerr << "After Date Loop" << std::endl;
                        if (temp_i < 0 || temp_i == it->second->schedule_datum.size()) {
                            if (over_due_flag == -1)
                                continue;
                            temp_i = over_due_flag;
                        }


                        zval *subarray;
                        MAKE_STD_ZVAL(subarray);
                        array_init(subarray);

                        add_assoc_double(subarray, "installment_amount", it->second->schedule_datum[temp_i].installment_amount);
                        add_assoc_double(subarray, "principle_installment_amount", it->second->schedule_datum[temp_i].principle_installment_amount);
                        add_assoc_double(subarray, "interest_installment_amount", it->second->schedule_datum[temp_i].interrest_installment_amount);

                        add_assoc_long(subarray, "installment_number", temp_i + 1);
                        add_assoc_string(subarray, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);

                        add_assoc_string(subarray, "first_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->getFirstInstallmentNumber()].schedule_date).c_str()), 1);
                        add_assoc_long(subarray, "last_installment_number", it->second->GetActualNumber_of_installment());
                        add_assoc_string(subarray, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[temp_i].schedule_date).c_str()), 1);


                        add_index_zval(array1, member_list[i], subarray);

                    }
                }

            }
        }

    } else {
        boost::gregorian::date d_date_frm(boost::gregorian::from_simple_string(s_date_frm));
        boost::gregorian::date d_date_to(boost::gregorian::from_simple_string(s_date_to));
        for (samity_loan_product_hash::iterator itt = branch_samity_loan_product_list[branch_id].begin(); itt != branch_samity_loan_product_list[branch_id].end(); itt++) {
            for (int i = 0; i < member_list.size(); i++) {

                zval *subarray;
                MAKE_STD_ZVAL(subarray);
                array_init(subarray);

                int index_flag = 0;
                for (loan_product_hash::iterator ittt = itt->second.begin(); ittt != itt->second.end(); ittt++)
                    for (loan_hash::iterator it = ittt->second.begin(); it != ittt->second.end(); it++) {

                        double installment_recoverable_before_date = 0;
                        double principle_recoverable_before_date = 0;
                        double interest_recoverable_before_date = 0;
                        if ((it->second->GetMember_id() == member_list[i]) && (it->second->GetLoan_fully_paid_date() > d_date_frm)) {

                            int temp_i = -1;

                            zval *subarray1;
                            MAKE_STD_ZVAL(subarray1);
                            array_init(subarray1);

                            int sub_index_flag = 0;
                            for (temp_i = it->second->getFirstInstallmentNumber(); temp_i < it->second->getLastInstallmentNumber(); temp_i++) {
                                //std::cerr << it->second->schedule_datum[temp_i].schedule_date << std::endl;
                                if (it->second->schedule_datum[temp_i].schedule_date <= d_date_frm) {
                                    installment_recoverable_before_date = installment_recoverable_before_date + it->second->schedule_datum[temp_i].installment_amount;
                                    principle_recoverable_before_date = principle_recoverable_before_date + it->second->schedule_datum[temp_i].principle_installment_amount;
                                    interest_recoverable_before_date = interest_recoverable_before_date + it->second->schedule_datum[temp_i].interrest_installment_amount;

                                }
                                if (it->second->schedule_datum[temp_i].schedule_date > d_date_frm && it->second->schedule_datum[temp_i].schedule_date <= d_date_to) {

                                    zval *subarray2;
                                    MAKE_STD_ZVAL(subarray2);
                                    array_init(subarray2);

                                    add_assoc_double(subarray2, "installment_amount", it->second->schedule_datum[temp_i].installment_amount);
                                    add_assoc_double(subarray2, "principle_installment_amount", it->second->schedule_datum[temp_i].principle_installment_amount);
                                    add_assoc_double(subarray2, "interest_installment_amount", it->second->schedule_datum[temp_i].interrest_installment_amount);

                                    add_assoc_long(subarray2, "installment_number", temp_i + 1);
                                    add_assoc_string(subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);

                                    add_assoc_string(subarray2, "first_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->getFirstInstallmentNumber()].schedule_date).c_str()), 1);
                                    add_assoc_long(subarray2, "last_installment_number", it->second->GetActualNumber_of_installment());
                                    add_assoc_string(subarray2, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[temp_i].schedule_date).c_str()), 1);

                                    sub_index_flag = 1;
                                    //add_index_zval(array1, member_list[i], subarray);
                                    add_index_zval(subarray1, temp_i, subarray2);
                                }

                            }
                            if (sub_index_flag == 1) {

                                add_assoc_double(subarray1, "installment_recoverable_before_date", installment_recoverable_before_date);
                                add_assoc_double(subarray1, "principle_recoverable_before_date", principle_recoverable_before_date);
                                add_assoc_double(subarray1, "interest_recoverable_before_date", interest_recoverable_before_date);
                                add_index_zval(subarray, it->second->GetId(), subarray1);
                                index_flag = 1;
                            }
                        }

                    }
                if (index_flag == 1)
                    add_index_zval(array1, member_list[i], subarray);
            }

        }
    }
    return array1;
}
 */

zval Loan_scheduler::get_member_wise_current_loan_information_just_before_date(std::vector<int> member_list, std::string s_date_frm, int branch_id, int product_id, int samity_id, std::string s_date_to) {

    //std::cerr << "get_member_wise_current_loan_information_just_before_date\n";
    //std::cerr << s_date_frm<<" "<<branch_id<<" "<<product_id;

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<int, float> samity_recov;
    samity_recov samity_recovery;

    if (samity_id && !s_date_to.empty()) {
        boost::gregorian::date d_date_frm(boost::gregorian::from_simple_string(s_date_frm));
        boost::gregorian::date d_date_to(boost::gregorian::from_simple_string(s_date_to));
        for (int i = 0; i < member_list.size(); i++) {
            zval subarray1;
//            MAKE_STD_ZVAL(subarray1);
            array_init(&subarray1);
            for (__gnu_cxx::hash_map<int, loan*>::iterator it = branch_samity_loan_product_list[branch_id][samity_id][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_id][product_id].end(); it++) {

                double installment_recoverable_before_date = 0;
                double principle_recoverable_before_date = 0;
                double interest_recoverable_before_date = 0;

                zval subarray2;
//                MAKE_STD_ZVAL(subarray2);
                array_init(&subarray2);
                if ((it->second->GetMember_id() == member_list[i]) && (it->second->GetDisburse_date() < d_date_to) && (it->second->GetLoan_fully_paid_date() > d_date_frm)) {
                    for (int temp_i = it->second->getFirstInstallmentNumber(); temp_i < it->second->getLastInstallmentNumber(); temp_i++) {
                        if (it->second->schedule_datum[temp_i].schedule_date > d_date_frm && it->second->schedule_datum[temp_i].schedule_date <= d_date_to) {

                            zval subarray;
//                            MAKE_STD_ZVAL(subarray);
                            array_init(&subarray);

                            add_assoc_double(&subarray, "installment_amount", it->second->schedule_datum[temp_i].installment_amount);
                            add_assoc_double(&subarray, "principle_installment_amount", it->second->schedule_datum[temp_i].principle_installment_amount);
                            add_assoc_double(&subarray, "interest_installment_amount", it->second->schedule_datum[temp_i].interrest_installment_amount);
                            add_assoc_long(&subarray, "installment_number", temp_i + 1);

                            //add_assoc_string(subarray, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                            std::string temp_last_date;
                            temp_last_date = this->get_last_schedule_date(it->second->GetId());
                            boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                            add_assoc_string(&subarray, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));


                            add_assoc_string(&subarray, "first_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->getFirstInstallmentNumber()].schedule_date).c_str()));
                            add_assoc_long(&subarray, "last_installment_number", it->second->GetActualNumber_of_installment());
                            add_assoc_string(&subarray, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[temp_i].schedule_date).c_str()));

                            add_index_zval(&subarray2, temp_i, &subarray);
                        }
                        if (it->second->schedule_datum[temp_i].schedule_date < d_date_frm) {
                            installment_recoverable_before_date = installment_recoverable_before_date + it->second->schedule_datum[temp_i].installment_amount;
                            principle_recoverable_before_date = principle_recoverable_before_date + it->second->schedule_datum[temp_i].principle_installment_amount;
                            interest_recoverable_before_date = interest_recoverable_before_date + it->second->schedule_datum[temp_i].interrest_installment_amount;
                        }
                    }
                    if (&subarray2) {
                        add_assoc_double(&subarray2, "installment_recoverable_before_date", installment_recoverable_before_date);
                        add_assoc_double(&subarray2, "principle_recoverable_before_date", principle_recoverable_before_date);
                        add_assoc_double(&subarray2, "interest_recoverable_before_date", interest_recoverable_before_date);
                        add_index_zval(&subarray1, it->second->GetId(), &subarray2);
                    }
                }

            }
            if (&subarray1)
                add_index_zval(&array1, member_list[i], &subarray1);
        }

    } else if (samity_id && s_date_to.empty()) {
        boost::gregorian::date d_date_frm(boost::gregorian::from_simple_string(s_date_frm));
        for (int i = 0; i < member_list.size(); i++) {
            zval subarray1;
//            MAKE_STD_ZVAL(subarray1);
            array_init(&subarray1);
            for (loan_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_id][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_id][product_id].end(); it++) {
                double installment_recoverable_before_date = 0;
                double principle_recoverable_before_date = 0;
                double interest_recoverable_before_date = 0;
                zval subarray2;
//                MAKE_STD_ZVAL(subarray2);
                array_init(&subarray2);
                if ((it->second->GetMember_id() == member_list[i]) && (it->second->GetDisburse_date() <= d_date_frm) && (it->second->GetLoan_fully_paid_date() > d_date_frm)) {
                    for (int temp_i = it->second->getFirstInstallmentNumber(); temp_i < it->second->getLastInstallmentNumber(); temp_i++) {
                        //std::cerr << it->second->schedule_datum[temp_i].schedule_date << std::endl;
                        if (it->second->schedule_datum[temp_i].schedule_date <= d_date_frm) {

                            zval subarray;
//                            MAKE_STD_ZVAL(subarray);
                            array_init(&subarray);

                            add_assoc_double(&subarray, "installment_amount", it->second->schedule_datum[temp_i].installment_amount);
                            add_assoc_double(&subarray, "principle_installment_amount", it->second->schedule_datum[temp_i].principle_installment_amount);
                            add_assoc_double(&subarray, "interest_installment_amount", it->second->schedule_datum[temp_i].interrest_installment_amount);
                            add_assoc_long(&subarray, "installment_number", temp_i + 1);

                            //add_assoc_string(subarray, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                            std::string temp_last_date;
                            temp_last_date = this->get_last_schedule_date(it->second->GetId());
                            boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                            add_assoc_string(&subarray, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                            add_assoc_string(&subarray, "first_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->getFirstInstallmentNumber()].schedule_date).c_str()));
                            add_assoc_long(&subarray, "last_installment_number", it->second->GetActualNumber_of_installment());
                            add_assoc_string(&subarray, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[temp_i].schedule_date).c_str()));

                            add_index_zval(&subarray2, temp_i, &subarray);
                        }
                        if (it->second->schedule_datum[temp_i].schedule_date <= d_date_frm) {
                            installment_recoverable_before_date = installment_recoverable_before_date + it->second->schedule_datum[temp_i].installment_amount;
                            principle_recoverable_before_date = principle_recoverable_before_date + it->second->schedule_datum[temp_i].principle_installment_amount;
                            interest_recoverable_before_date = interest_recoverable_before_date + it->second->schedule_datum[temp_i].interrest_installment_amount;
                        }
                    }

                    if (&subarray2) {
                        add_assoc_double(&subarray2, "installment_recoverable_before_date", installment_recoverable_before_date);
                        add_assoc_double(&subarray2, "principle_recoverable_before_date", principle_recoverable_before_date);
                        add_assoc_double(&subarray2, "interest_recoverable_before_date", interest_recoverable_before_date);
                        add_index_zval(&subarray1, it->second->GetId(), &subarray2);
                    }
                }
            }
            if (&subarray1)
                add_index_zval(&array1, member_list[i], &subarray1);
        }

    } else if (s_date_to.empty()) {
        boost::gregorian::date d_date_frm(boost::gregorian::from_simple_string(s_date_frm));
        for (int i = 0; i < member_list.size(); i++) {
            zval subarray1;
//            MAKE_STD_ZVAL(subarray1);
            array_init(&subarray1);
            for (__gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > >::iterator it1 = branch_samity_loan_product_list[branch_id].begin(); it1 != branch_samity_loan_product_list[branch_id].end(); it1++) {
                for (__gnu_cxx::hash_map<int, loan*>::iterator it = it1->second[product_id].begin(); it != it1->second[product_id].end(); it++) {

                    double installment_recoverable_before_date = 0;
                    double principle_recoverable_before_date = 0;
                    double interest_recoverable_before_date = 0;

                    zval subarray2;
//                    MAKE_STD_ZVAL(subarray2);
                    array_init(&subarray2);
                    if ((it->second->GetMember_id() == member_list[i]) && (it->second->GetDisburse_date() <= d_date_frm) && (it->second->GetLoan_fully_paid_date() > d_date_frm)) {
                        for (int temp_i = it->second->getFirstInstallmentNumber(); temp_i < it->second->getLastInstallmentNumber(); temp_i++) {
                            //std::cerr << it->second->schedule_datum[temp_i].schedule_date << std::endl;
                            if (it->second->schedule_datum[temp_i].schedule_date <= d_date_frm) {
                                zval subarray;
//                                MAKE_STD_ZVAL(subarray);
                                array_init(&subarray);

                                add_assoc_double(&subarray, "installment_amount", it->second->schedule_datum[temp_i].installment_amount);
                                add_assoc_double(&subarray, "principle_installment_amount", it->second->schedule_datum[temp_i].principle_installment_amount);
                                add_assoc_double(&subarray, "interest_installment_amount", it->second->schedule_datum[temp_i].interrest_installment_amount);
                                add_assoc_long(&subarray, "installment_number", temp_i + 1);

                                add_assoc_string(&subarray, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()));
                                std::string temp_last_date;
                                temp_last_date = this->get_last_schedule_date(it->second->GetId());
                                boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                                add_assoc_string(&subarray, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                                add_assoc_string(&subarray, "first_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->getFirstInstallmentNumber()].schedule_date).c_str()));
                                add_assoc_long(&subarray, "last_installment_number", it->second->GetActualNumber_of_installment());
                                add_assoc_string(&subarray, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[temp_i].schedule_date).c_str()));

                                add_index_zval(&subarray2, temp_i, &subarray);
                            }
                            if (it->second->schedule_datum[temp_i].schedule_date <= d_date_frm) {
                                installment_recoverable_before_date = installment_recoverable_before_date + it->second->schedule_datum[temp_i].installment_amount;
                                principle_recoverable_before_date = principle_recoverable_before_date + it->second->schedule_datum[temp_i].principle_installment_amount;
                                interest_recoverable_before_date = interest_recoverable_before_date + it->second->schedule_datum[temp_i].interrest_installment_amount;
                            }
                        }
                        if (&subarray2) {
                            add_assoc_double(&subarray2, "installment_recoverable_before_date", installment_recoverable_before_date);
                            add_assoc_double(&subarray2, "principle_recoverable_before_date", principle_recoverable_before_date);
                            add_assoc_double(&subarray2, "interest_recoverable_before_date", interest_recoverable_before_date);
                            add_index_zval(&subarray1, it->second->GetId(), &subarray2);
                        }
                    }
                }
            }
            if (&subarray1)
                add_index_zval(&array1, member_list[i], &subarray1);
        }

    }

    return array1;
}

///////////////////////////////////////////////////////////////////////

zval Loan_scheduler::get_samity_wise_expired_loan_information_before_date(std::vector<int> samity_list, std::string s_date, int branch_id, int product_id) {

    boost::gregorian::date d_date(boost::gregorian::from_simple_string(s_date));

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<int, float> samity_recov;
    samity_recov samity_recovery;

    if (product_id) {

        zval subarray2;
//        MAKE_STD_ZVAL(subarray2);
        array_init(&subarray2);

        zval subarray3;
//        MAKE_STD_ZVAL(subarray3);
        array_init(&subarray3);

        for (int i = 0; i < samity_list.size(); i++) {

            int uu = 0;

            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;
            for (loan_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]][product_id].end(); it++) {
                samity_idd = samity_list[i];
                zval subarray;
//                MAKE_STD_ZVAL(subarray);
                array_init(&subarray);                
                std::string temp_last_date;
                temp_last_date = this->get_last_schedule_date(it->second->GetId());
                boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                //std::cerr << "loan_id before all: " << it->second->GetId() << std::endl;
                //std::cerr << "MURADXXX " << it->second->transfer_out_date  << "##" << it->second->GetLoan_fully_paid_date() << "PPPPPPP" << std::endl;        


                if ((it->second->GetLoan_fully_paid_date() > d_date) && temp_last_date_cus < d_date) {
                    //if (it->second->GetIs_loan_fully_paid() == 0) {
                    //std::cerr << "loan_id before: " << it->second->GetId() << std::endl;
                    no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                    cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it->second->GetLoan_amount();

                    zval subarray1;
//                    MAKE_STD_ZVAL(subarray1);
                    array_init(&subarray1);

                    add_assoc_long(&subarray1, "loan_id", it->second->GetId());
                    add_assoc_long(&subarray1, "samity_id", samity_idd);
                    add_assoc_long(&subarray1, "installment_no", it->second->GetActualNumber_of_installment());
                    add_assoc_double(&subarray1, "installment_amount", it->second->schedule_datum[it->second->getFirstInstallmentNumber()].installment_amount);

                    add_assoc_double(&subarray1, "principle_installment_amount", it->second->schedule_datum[it->second->getFirstInstallmentNumber()].principle_installment_amount);
                    add_assoc_double(&subarray1, "interrest_installment_amount", it->second->schedule_datum[it->second->getFirstInstallmentNumber()].interrest_installment_amount);
                    add_assoc_double(&subarray1, "last_principle_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].principle_installment_amount);
                    add_assoc_double(&subarray1, "last_interrest_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].interrest_installment_amount);

                    add_assoc_double(&subarray1, "last_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].installment_amount);

                    // add_assoc_string(subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);



                    //std::string temp_last_date;
                    //temp_last_date=this->get_last_schedule_date(it->second->GetId());
                    //boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                    add_assoc_string(&subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                    //add_index_zval(subarray, it->second->GetId(), subarray1);
                    add_index_zval(&subarray2, it->second->GetId(), &subarray1);
                    //add_index_zval(array1,samity_list[i],subarray);
                    //std::cerr << "loan_id after: " << it->second->GetId() << std::endl;
                }
                //add_index_zval(subarray3,samity_list[i],subarray);




            }

            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);


            add_assoc_long(&subarray, "samity_id", samity_idd);
            add_assoc_long(&subarray, "no_cumilative_expaired_borrower", no_cumilative_expaired_borrower);
            add_assoc_double(&subarray, "cumilative_expaired_borrower_amount", cumilative_expaired_borrower_amount);
            add_index_zval(&subarray3, samity_list[i], &subarray);
            //add_index_zval(array1, samity_list[i], subarray);
        }
        add_assoc_zval(&array1, "samities", &subarray3);
        add_assoc_zval(&array1, "loans", &subarray2);

    } else {

        zval subarray2;
//        MAKE_STD_ZVAL(subarray2);
        array_init(&subarray2);

        zval subarray3;
//        MAKE_STD_ZVAL(subarray3);
        array_init(&subarray3);

        for (int i = 0; i < samity_list.size(); i++) {


            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;
            for (loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]].end(); it++) {
                samity_idd = samity_list[i];
                for (loan_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {

                    std::string temp_last_date;
                    temp_last_date = this->get_last_schedule_date(it1->second->GetId());
                    boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));


                    if ((it1->second->GetLoan_fully_paid_date() > d_date) && temp_last_date_cus < d_date) {
                        //if (it1->second->GetIs_loan_fully_paid() == 0) {


                        no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                        cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it1->second->GetLoan_amount();

                        zval subarray1;
//                        MAKE_STD_ZVAL(subarray1);
                        array_init(&subarray1);

                        add_assoc_long(&subarray1, "loan_id", it1->second->GetId());
                        add_assoc_long(&subarray1, "samity_id", samity_idd);
                        add_assoc_long(&subarray1, "installment_no", it1->second->GetActualNumber_of_installment());
                        add_assoc_double(&subarray1, "installment_amount", it1->second->schedule_datum[it1->second->getFirstInstallmentNumber()].installment_amount);

                        add_assoc_double(&subarray1, "principle_installment_amount", it1->second->schedule_datum[it1->second->getFirstInstallmentNumber()].principle_installment_amount);
                        add_assoc_double(&subarray1, "interrest_installment_amount", it1->second->schedule_datum[it1->second->getFirstInstallmentNumber()].interrest_installment_amount);
                        add_assoc_double(&subarray1, "last_principle_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].principle_installment_amount);
                        add_assoc_double(&subarray1, "last_interrest_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].interrest_installment_amount);


                        add_assoc_double(&subarray1, "last_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].installment_amount);

                        //add_assoc_string(subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                        //std::string temp_last_date;
                        //temp_last_date=this->get_last_schedule_date(it1->second->GetId());
                        //boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                        add_assoc_string(&subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));



                        //add_index_zval(subarray, it1->second->GetId(), subarray1);
                        add_index_zval(&subarray2, it1->second->GetId(), &subarray1);
                        //add_index_zval(array1,samity_list[i],subarray);

                    }
                    //add_index_zval(array1,samity_list[i],subarray);




                }
            }


            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            add_assoc_long(&subarray, "samity_id", samity_idd);
            add_assoc_long(&subarray, "no_cumilative_expaired_borrower", no_cumilative_expaired_borrower);
            add_assoc_double(&subarray, "cumilative_expaired_borrower_amount", cumilative_expaired_borrower_amount);
            add_index_zval(&subarray3, samity_list[i], &subarray);
            //add_index_zval(array1, samity_list[i], subarray);
        }

        add_assoc_zval(&array1, "samities", &subarray3);
        add_assoc_zval(&array1, "loans", &subarray2);

    }
    return array1;
}

zval Loan_scheduler::get_samity_wise_expired_loan_information_between_date(std::vector<int> samity_list, std::string s_date_from, std::string s_date_to, int branch_id, int product_id) {

    //std::cerr << "get_samity_wise_expired_loan_information_between_date" << std::endl;
    boost::gregorian::date d_date_from(boost::gregorian::from_simple_string(s_date_from));
    boost::gregorian::date d_date_to(boost::gregorian::from_simple_string(s_date_to));

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<int, float> samity_recov;
    samity_recov samity_recovery;

    if (product_id) {
        int flagg = 0;
        for (int i = 0; i < samity_list.size(); i++) {
            //std::cerr<<branch_id<<" "<< samity_list[i]<<" "<<product_id<<std::endl;
            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            zval subarray2;
//            MAKE_STD_ZVAL(subarray2);
            array_init(&subarray2);
            //int uu=0;

            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;
            //std::cerr << "Before loan_hash\n";
            for (loan_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]][product_id].end(); it++) {
                samity_idd = samity_list[i];
                //std::cerr << "loan_hash \n"<<it->second->GetActualNumber_of_installment()<<" ";
                //std::cerr << it->second->GetId();
                ////std::cerr << " " << it->second->GetIs_loan_fully_paid() << " " << it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date << std::endl;
                //zval *subarray;
                //MAKE_STD_ZVAL(subarray);
                //array_init(subarray);
                if ((it->second->GetLoan_fully_paid_date() > d_date_to && it->second->GetLast_repayment_date() <= d_date_from) || (it->second->GetLoan_fully_paid_date() >= d_date_from && it->second->GetLast_repayment_date() < d_date_from)) {
                    //std::cerr << "before inner condition" << std::endl;
                    if (it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date < d_date_to) {
                        zval subarray3;
//                        MAKE_STD_ZVAL(subarray3);
                        array_init(&subarray3);
                        for (int j = it->second->getFirstInstallmentNumber(); j < it->second->getLastInstallmentNumber(); j++) {
                            if (it->second->schedule_datum[j].schedule_date >= d_date_from && it->second->schedule_datum[j].schedule_date <= d_date_to) {
                                //std::cerr << "inner condition" << std::endl;
                                no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                                cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it->second->GetLoan_amount();

                                zval subarray1;
//                                MAKE_STD_ZVAL(subarray1);
                                array_init(&subarray1);

                                add_assoc_long(&subarray1, "loan_id", it->second->GetId());

                                add_assoc_long(&subarray1, "installment_no", j);
                                add_assoc_double(&subarray1, "installment_amount", it->second->schedule_datum[j].installment_amount);

                                add_assoc_double(&subarray1, "last_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].installment_amount);

                                //add_assoc_string(subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                                std::string temp_last_date;
                                temp_last_date = this->get_last_schedule_date(it->second->GetId());
                                boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                                add_assoc_string(&subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));



                                //add_index_zval(subarray, it->second->GetId(), subarray1);
                                flagg++;
                                add_index_zval(&subarray3, j, &subarray1);
                            }
                        }
                        add_index_zval(&subarray2, it->second->GetId(), &subarray3);
                    }
                }
                //add_index_zval(subarray3,samity_list[i],subarray);

                //std::cerr << "Out of loop" << std::endl;


            }
            //if (flagg) {
            //std::cerr << "Out Outer of loop" << std::endl;
            add_assoc_long(&subarray, "samity_id", samity_idd);
            add_assoc_long(&subarray, "no_cumilative_expaired_borrower", no_cumilative_expaired_borrower);
            add_assoc_double(&subarray, "cumilative_expaired_borrower_amount", cumilative_expaired_borrower_amount);
            add_assoc_zval(&subarray, "loans", &subarray2);
            add_index_zval(&array1, samity_list[i], &subarray);
            //}
            //std::cerr << "Out Outer of loop" << std::endl;
        }
    } else {
        for (int i = 0; i < samity_list.size(); i++) {

            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            zval subarray2;
//            MAKE_STD_ZVAL(subarray2);
            array_init(&subarray2);

            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;
            for (loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]].end(); it++) {
                samity_idd = samity_list[i];
                for (loan_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {

                    //zval *subarray;
                    //MAKE_STD_ZVAL(subarray);
                    //array_init(subarray);
                    if ((it1->second->GetLoan_fully_paid_date() > d_date_to && it1->second->GetLast_repayment_date() < d_date_from) || (it1->second->GetLoan_fully_paid_date() >= d_date_from && it1->second->GetLast_repayment_date() < d_date_from)) {
                        //std::cerr << "before inner condition" << std::endl;
                        if (it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].schedule_date < d_date_to) {
                            zval subarray3;
//                            MAKE_STD_ZVAL(subarray3);
                            array_init(&subarray3);
                            for (int j = it1->second->getFirstInstallmentNumber(); j < it1->second->getLastInstallmentNumber(); j++) {
                                if (it1->second->schedule_datum[j].schedule_date >= d_date_from && it1->second->schedule_datum[j].schedule_date <= d_date_to) {
                                    //std::cerr << "inner condition" << std::endl;
                                    no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                                    cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it1->second->GetLoan_amount();

                                    zval subarray1;
//                                    MAKE_STD_ZVAL(subarray1);
                                    array_init(&subarray1);

                                    add_assoc_long(&subarray1, "loan_id", it1->second->GetId());

                                    add_assoc_long(&subarray1, "installment_no", j);
                                    add_assoc_double(&subarray1, "installment_amount", it1->second->schedule_datum[j].installment_amount);

                                    add_assoc_double(&subarray1, "last_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].installment_amount);

                                    //add_assoc_string(subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                                    std::string temp_last_date;
                                    temp_last_date = this->get_last_schedule_date(it1->second->GetId());
                                    boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                                    add_assoc_string(&subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));


                                    //add_index_zval(subarray, it->second->GetId(), subarray1);
                                    //flagg++;
                                    add_index_zval(&subarray3, j, &subarray1);
                                }
                            }
                            add_index_zval(&subarray2, it1->second->GetId(), &subarray3);
                        }
                    }
                    //add_index_zval(array1,samity_list[i],subarray);




                }
            }
            add_assoc_long(&subarray, "samity_id", samity_idd);
            add_assoc_long(&subarray, "no_cumilative_expaired_borrower", no_cumilative_expaired_borrower);
            add_assoc_double(&subarray, "cumilative_expaired_borrower_amount", cumilative_expaired_borrower_amount);
            add_assoc_zval(&subarray, "loans", &subarray2);
            add_index_zval(&array1, samity_list[i], &subarray);
        }

    }
    //std::cerr << "before return" << std::endl;
    return array1;
}

zval Loan_scheduler::get_samity_wise_current_loan_information_before_date(std::vector<int> samity_list, std::string s_date, int branch_id, int product_id) {

    boost::gregorian::date d_date(boost::gregorian::from_simple_string(s_date));

    double cumilative_branch_loan_amount = 0;

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<int, float> samity_recov;
    samity_recov samity_recovery;

    if (product_id) {
        for (int i = 0; i < samity_list.size(); i++) {

            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;
            for (loan_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]][product_id].end(); it++) {

                zval subarray1;
//                MAKE_STD_ZVAL(subarray1);
                array_init(&subarray1);

                int flag = 0;
                //std::cerr << it->second->GetId() << " " << it->second->GetLoan_fully_paid_date() << " " << d_date << std::endl;
                if (it->second->GetLoan_fully_paid_date() >= d_date) {
                    //if(it->second.schedule_datum[0].schedule_date<d_date){
                    no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                    int temp_i = 0;
                    for (temp_i = it->second->getFirstInstallmentNumber(); temp_i < it->second->getLastInstallmentNumber(); temp_i++) {
                        if (it->second->schedule_datum[temp_i].schedule_date < d_date) {

                            flag = 1;
                            cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it->second->schedule_datum[temp_i].installment_amount;
                            samity_idd = samity_list[i];

                            zval subarray2;
//                            MAKE_STD_ZVAL(subarray2);
                            array_init(&subarray2);

                            add_assoc_long(&subarray2, "loan_id", it->second->GetId());
                            add_assoc_double(&subarray1, "loan_amount", it->second->GetLoan_amount());
                            add_assoc_double(&subarray1, "total_payable_amount", it->second->GetTotal_payable_amount());

                            add_assoc_long(&subarray2, "installment_no", temp_i + 1);
                            add_assoc_double(&subarray2, "installment_amount", it->second->schedule_datum[temp_i].installment_amount);
                            add_assoc_double(&subarray2, "principle_installment_amount", it->second->schedule_datum[temp_i].principle_installment_amount);

                            add_assoc_string(&subarray2, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[temp_i].schedule_date).c_str()));

                            add_assoc_long(&subarray2, "total_installment_no", it->second->GetActualNumber_of_installment());
                            add_assoc_double(&subarray2, "last_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].installment_amount);

                            //add_assoc_string(subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                            std::string temp_last_date;
                            temp_last_date = this->get_last_schedule_date(it->second->GetId());
                            boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                            add_assoc_string(&subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));


                            add_index_zval(&subarray1, temp_i + 1, &subarray2);
                            //add_index_zval(array1,samity_list[i],subarray);
                        }
                    }
                    //}
                }
                //add_index_zval(subarray3,samity_list[i],subarray);
                if (flag == 1) {
                    add_index_zval(&subarray, it->second->GetId(), &subarray1);
                }



            }
            if (samity_idd) {
                add_assoc_long(&subarray, "samity_id", samity_idd);
                add_assoc_long(&subarray, "no_current_borrower", no_cumilative_expaired_borrower);
                add_assoc_double(&subarray, "cumilative_current_amount", cumilative_expaired_borrower_amount);
                cumilative_branch_loan_amount = cumilative_branch_loan_amount + cumilative_expaired_borrower_amount;
                add_index_zval(&array1, samity_list[i], &subarray);
            }
        }
    } else {
        for (int i = 0; i < samity_list.size(); i++) {

            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;
            for (loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]].end(); it++) {

                for (loan_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
                    int flag = 0;
                    zval subarray1;
//                    MAKE_STD_ZVAL(subarray1);
                    array_init(&subarray1);



                    if (it1->second->GetLoan_fully_paid_date() >= d_date) {
                        if (it1->second->schedule_datum[it1->second->getFirstInstallmentNumber()].schedule_date < d_date) {
                            int temp_i = 0;
                            for (temp_i = it1->second->getFirstInstallmentNumber(); temp_i < it1->second->getLastInstallmentNumber(); temp_i++) {
                                if (it1->second->schedule_datum[temp_i].schedule_date < d_date) {

                                    flag = 1;
                                    samity_idd = samity_list[i];
                                    no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                                    cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it1->second->schedule_datum[temp_i].installment_amount;

                                    zval subarray2;
//                                    MAKE_STD_ZVAL(subarray2);
                                    array_init(&subarray2);

                                    add_assoc_long(&subarray2, "loan_id", it1->second->GetId());
                                    add_assoc_double(&subarray1, "loan_amount", it1->second->GetLoan_amount());
                                    add_assoc_double(&subarray1, "total_payable_amount", it1->second->GetTotal_payable_amount());

                                    add_assoc_long(&subarray2, "installment_no", temp_i + 1);
                                    add_assoc_double(&subarray2, "installment_amount", it1->second->schedule_datum[temp_i].installment_amount);
                                    add_assoc_double(&subarray2, "principle_installment_amount", it1->second->schedule_datum[temp_i].principle_installment_amount);
                                    add_assoc_string(&subarray2, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[temp_i].schedule_date).c_str()));

                                    add_assoc_long(&subarray2, "total_installment_no", it1->second->GetActualNumber_of_installment());
                                    add_assoc_double(&subarray2, "last_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].installment_amount);

                                    //add_assoc_string(subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                                    std::string temp_last_date;
                                    temp_last_date = this->get_last_schedule_date(it1->second->GetId());
                                    boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                                    add_assoc_string(&subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                                    add_index_zval(&subarray1, temp_i + 1, &subarray2);
                                    //add_index_zval(array1,samity_list[i],subarray);
                                }
                            }
                        }
                    }
                    //add_index_zval(array1,samity_list[i],subarray);

                    if (flag == 1) {
                        add_index_zval(&subarray, it1->second->GetId(), &subarray1);
                    }


                }
            }
            if (samity_idd) {
                add_assoc_long(&subarray, "samity_id", samity_idd);
                add_assoc_long(&subarray, "no_current_borrower", no_cumilative_expaired_borrower);
                add_assoc_double(&subarray, "cumilative_current_amount", cumilative_expaired_borrower_amount);
                cumilative_branch_loan_amount = cumilative_branch_loan_amount + cumilative_expaired_borrower_amount;
                add_index_zval(&array1, samity_list[i], &subarray);
            }
        }

    }
    add_assoc_double(&array1, "cumilative_branch_loan_amount", cumilative_branch_loan_amount);
    return array1;
}

//////

zval Loan_scheduler::get_samity_wise_current_loan_information_just_before_date_consider_fully_paid(std::vector<int> samity_list, std::string s_date, int branch_id, int product_id) {

    boost::gregorian::date d_date(boost::gregorian::from_simple_string(s_date));

    //std::cerr << "In Just Before Date" << std::endl;
    //std::cerr << "Input:" << s_date << " " << branch_id << " " << product_id << std::endl;
    double cumilative_branch_loan_amount = 0;

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > >branch_samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<int, float> samity_recov;
    samity_recov samity_recovery;

    if (product_id && branch_id) {
        //std::cerr << "In Just Before Date: Branch,Product" << std::endl;
        for (int i = 0; i < samity_list.size(); i++) {

            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;
            for (loan_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]][product_id].end(); it++) {

                zval subarray1;
//                MAKE_STD_ZVAL(subarray1);
                array_init(&subarray1);
                if(*this->system_date<it->second->GetDisburse_date()){
                    continue;
                }
                int flag = 0;
                //std::cerr << "loan_id" << it->second->GetId() << std::endl;
                if (it->second->GetLoan_fully_paid_date() >= d_date && it->second->GetLast_repayment_date() >= d_date) {
                    //if(it->second.schedule_datum[0].schedule_date<d_date){                    
                    no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                    int temp_i = 0;
                    for (temp_i = it->second->getFirstInstallmentNumber(); temp_i < it->second->getLastInstallmentNumber(); temp_i++) {
                        if (it->second->schedule_datum[temp_i].schedule_date > d_date) {
                            if ((it->second->schedule_datum[0].schedule_date == it->second->schedule_datum[temp_i].schedule_date) && (it->second->GetDisburse_date() < d_date) && (d_date < it->second->schedule_datum[0].schedule_date) && d_date == it->second->GetLoan_fully_paid_date()) {
                                temp_i = 0;
                            } else {
                                temp_i = temp_i - 1;
                            }
                            break;
                        } else if (it->second->schedule_datum[temp_i].schedule_date == d_date) {
                            break;
                        }
                    }
                    if (temp_i == it->second->getLastInstallmentNumber() || temp_i == it->second->getFirstInstallmentNumber() - 1) {
                        continue;
                    }


                    flag = 1;
                    cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it->second->schedule_datum[temp_i].installment_amount;
                    samity_idd = samity_list[i];


                    //zval *subarray2;
                    //MAKE_STD_ZVAL(subarray2);
                    //array_init(subarray2);

                    add_assoc_long(&subarray1, "loan_id", it->second->GetId());
                    add_assoc_long(&subarray1, "product_id", it->second->GetProduct_id());
                    add_assoc_double(&subarray1, "loan_amount", it->second->GetLoan_amount());
                    add_assoc_double(&subarray1, "total_payable_amount", it->second->GetTotal_payable_amount());
                    add_assoc_long(&subarray1, "cycle", it->second->GetCycle());
                    add_assoc_string(&subarray1, "loan_type",const_cast<char*> (it->second->GetLoan_type().c_str()));
                    add_assoc_string(&subarray1, "repayment_frequency",const_cast<char*> (it->second->GetRepayment_frequency().c_str()));
                    
                    add_assoc_long(&subarray1, "installment_no", temp_i + 1);
                    add_assoc_double(&subarray1, "installment_amount", it->second->schedule_datum[temp_i].installment_amount);
                    add_assoc_double(&subarray1, "principle_installment_amount", it->second->schedule_datum[temp_i].principle_installment_amount);
                    add_assoc_double(&subarray1, "interrest_installment_amount", it->second->schedule_datum[temp_i].interrest_installment_amount);


                    add_assoc_string(&subarray1, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[temp_i].schedule_date).c_str()));

                    add_assoc_long(&subarray1, "total_installment_no", it->second->GetActualNumber_of_installment());
                    add_assoc_double(&subarray1, "last_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].installment_amount);
                    add_assoc_double(&subarray1, "last_principle_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].principle_installment_amount);
                    add_assoc_double(&subarray1, "last_interrest_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].interrest_installment_amount);



                    //add_assoc_string(subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                    std::string temp_last_date;
                    temp_last_date = this->get_last_schedule_date(it->second->GetId());
                    boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                    add_assoc_string(&subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                    double current_principle_recoverable = 0;
                    double current_interrest_recoverable = 0;
                    double current_installment_recoverable = 0;
                    for (int i = it->second->getFirstInstallmentNumber(); i < temp_i + 1; i++) {
                        current_principle_recoverable = current_principle_recoverable + it->second->schedule_datum[i].principle_installment_amount;
                        current_interrest_recoverable = current_interrest_recoverable + it->second->schedule_datum[i].interrest_installment_amount;
                        current_installment_recoverable = current_installment_recoverable + it->second->schedule_datum[i].installment_amount;
                    }
                    add_assoc_double(&subarray1, "current_installment_recoverable", current_installment_recoverable);
                    add_assoc_double(&subarray1, "current_principle_recoverable", current_principle_recoverable);
                    add_assoc_double(&subarray1, "current_interrest_recoverable", current_interrest_recoverable);
                    //add_index_zval(subarray,temp_i+1,subarray1);
                    //add_index_zval(array1,samity_list[i],subarray);

                    //}
                }
                //add_index_zval(subarray3,samity_list[i],subarray);
                if (flag == 1) {
                    add_index_zval(&subarray, it->second->GetId(), &subarray1);
                }



            }
            if (samity_idd) {
                add_assoc_long(&subarray, "samity_id", samity_idd);
                add_assoc_long(&subarray, "no_current_borrower", no_cumilative_expaired_borrower);
                add_assoc_double(&subarray, "cumilative_current_amount", cumilative_expaired_borrower_amount);
                cumilative_branch_loan_amount = cumilative_branch_loan_amount + cumilative_expaired_borrower_amount;
                add_index_zval(&array1, samity_list[i], &subarray);
            }
        }
    } else if (branch_id) {
        //std::cerr << "In Just Before Date: Branch 1" << std::endl;
        for (int i = 0; i < samity_list.size(); i++) {
            //std::cerr << "In Just Before Date: Branch 2 " << samity_list[i] << std::endl;
            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;
            //for (branch_samity_loan_product_hash::iterator it2 = branch_samity_loan_product_list.begin(); it2 != branch_samity_loan_product_list.end(); it2++) {
            //std::cerr << "In Just Before Date: Branch 3" << std::endl;
            //for (loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]].end(); it++) {
            //std::cerr << "In Just Before Date: Branch 4 " << branch_id << " " << samity_list[i] << std::endl;
            //for (loan_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
            for (loan_hash::iterator it1 = branch_samity_loan_list[branch_id][samity_list[i]].begin(); it1 != branch_samity_loan_list[branch_id][samity_list[i]].end(); it1++) {
                //std::cerr << "In Just Before Date: Branch 5" << std::endl;
                int flag = 0;
                zval subarray1;
//                MAKE_STD_ZVAL(subarray1);
                array_init(&subarray1);

                //std::cerr << "loan_id " << it1->second->GetId() <<std::endl;
                //std::cerr<<it1->second->GetLoan_fully_paid_date()<<" "<<d_date<<" "<<it1->second->GetLast_repayment_date()<<std::endl;
                if(*this->system_date<it1->second->GetDisburse_date()){
                    continue;
                }
                if (it1->second->GetLoan_fully_paid_date() >= d_date && it1->second->GetLast_repayment_date() >= d_date) {
                    //std::cerr << "In Just Before Date: Branch 6 "<<it1->second->GetId()<<" "<<it1->second->schedule_datum[0].schedule_date<<" "<<d_date << std::endl;
                    if (it1->second->schedule_datum[0].schedule_date <= d_date) {
                        //std::cerr << "loan_id" << it1->second->GetId();
                        //std::cerr << "In Just Before Date: Branch 7" << std::endl;
                        int temp_i = 0;
                        for (temp_i = it1->second->getFirstInstallmentNumber(); temp_i < it1->second->getLastInstallmentNumber(); temp_i++) {
                            if (it1->second->schedule_datum[temp_i].schedule_date > d_date) {
                                if ((it1->second->schedule_datum[0].schedule_date == it1->second->schedule_datum[temp_i].schedule_date) && (it1->second->GetDisburse_date() < d_date) && (d_date < it1->second->schedule_datum[0].schedule_date) && d_date == it1->second->GetLoan_fully_paid_date()) {
                                    temp_i = 0;
                                } else {
                                    temp_i = temp_i - 1;
                                }
                                break;
                                //temp_i = temp_i - 1;
                                //break;
                            } else if (it1->second->schedule_datum[temp_i].schedule_date == d_date) {
                                break;
                            }
                        }
                        if (temp_i == it1->second->getLastInstallmentNumber() || temp_i == it1->second->getFirstInstallmentNumber() - 1) {
                            continue;
                        }

                        flag = 1;
                        samity_idd = samity_list[i];
                        no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                        cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it1->second->schedule_datum[temp_i].installment_amount;

                        //zval *subarray2;
                        //MAKE_STD_ZVAL(subarray2);
                        //array_init(subarray2);

                        add_assoc_long(&subarray1, "loan_id", it1->second->GetId());
                        add_assoc_double(&subarray1, "loan_amount", it1->second->GetLoan_amount());
                        add_assoc_double(&subarray1, "total_payable_amount", it1->second->GetTotal_payable_amount());
                        add_assoc_long(&subarray1, "cycle", it1->second->GetCycle());
                        //std::cerr<<"Loan Cycle "<<it1->second->GetCycle() <<std::endl;
                        add_assoc_string(&subarray1, "loan_type",const_cast<char*> (it1->second->GetLoan_type().c_str()));
                        add_assoc_string(&subarray1, "repayment_frequency",const_cast<char*> (it1->second->GetRepayment_frequency().c_str()));
                        
                        add_assoc_long(&subarray1, "product_id", it1->second->GetProduct_id());
                        add_assoc_long(&subarray1, "installment_no", temp_i + 1);
                        add_assoc_double(&subarray1, "installment_amount", it1->second->schedule_datum[temp_i].installment_amount);
                        add_assoc_double(&subarray1, "principle_installment_amount", it1->second->schedule_datum[temp_i].principle_installment_amount);
                        add_assoc_double(&subarray1, "interrest_installment_amount", it1->second->schedule_datum[temp_i].interrest_installment_amount);

                        //std::cerr << "In Just Before Date: before date convert1 " <<it1->second->GetId()<<" "<< temp_i << " " << it1->second->schedule_datum[temp_i].schedule_date << std::endl;
                        add_assoc_string(&subarray1, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[temp_i].schedule_date).c_str()));

                        add_assoc_long(&subarray1, "total_installment_no", it1->second->GetActualNumber_of_installment());
                        add_assoc_double(&subarray1, "last_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].installment_amount);
                        add_assoc_double(&subarray1, "last_principle_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].principle_installment_amount);
                        add_assoc_double(&subarray1, "last_interrest_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].interrest_installment_amount);

                        //std::cerr << "In Just Before Date: before date convert2" << std::endl;
                        //add_assoc_string(subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                        std::string temp_last_date;
                        temp_last_date = this->get_last_schedule_date(it1->second->GetId());
                        boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                        add_assoc_string(&subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));
                        double current_principle_recoverable = 0;
                        double current_interrest_recoverable = 0;
                        double current_installment_recoverable = 0;
                        for (int i = 0; i < temp_i + 1; i++) {
                            current_principle_recoverable = current_principle_recoverable + it1->second->schedule_datum[i].principle_installment_amount;
                            current_interrest_recoverable = current_interrest_recoverable + it1->second->schedule_datum[i].interrest_installment_amount;
                            current_installment_recoverable = current_installment_recoverable + it1->second->schedule_datum[i].installment_amount;
                        }
                        add_assoc_double(&subarray1, "current_installment_recoverable", current_installment_recoverable);
                        add_assoc_double(&subarray1, "current_principle_recoverable", current_principle_recoverable);
                        add_assoc_double(&subarray1, "current_interrest_recoverable", current_interrest_recoverable);
                        //add_index_zval(subarray,temp_i+1,subarray1);
                        //add_index_zval(array1,samity_list[i],subarray);

                    }
                }
                //add_index_zval(array1,samity_list[i],subarray);

                if (flag == 1) {
                    add_index_zval(&subarray, it1->second->GetId(), &subarray1);
                }


            }
            //}
            // }
            if (samity_idd) {
                add_assoc_long(&subarray, "samity_id", samity_idd);
                add_assoc_long(&subarray, "no_current_borrower", no_cumilative_expaired_borrower);
                add_assoc_double(&subarray, "cumilative_current_amount", cumilative_expaired_borrower_amount);
                cumilative_branch_loan_amount = cumilative_branch_loan_amount + cumilative_expaired_borrower_amount;
                add_index_zval(&array1, samity_list[i], &subarray);
            }
        }

    } else {
        //std::cerr << "In Just Before Date: Empty" << std::endl;
        for (int i = 0; i < samity_list.size(); i++) {

            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;
            for (loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]].end(); it++) {

                for (loan_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
                    int flag = 0;
                    zval subarray1;
//                    MAKE_STD_ZVAL(subarray1);
                    array_init(&subarray1);



                    if (it1->second->GetLoan_fully_paid_date() >= d_date && it1->second->GetLast_repayment_date() >= d_date) {
                        if (it1->second->schedule_datum[0].schedule_date <= d_date) {
                            int temp_i = 0;
                            for (temp_i = it1->second->getFirstInstallmentNumber(); temp_i < it1->second->getLastInstallmentNumber(); temp_i++) {
                                if (it1->second->schedule_datum[temp_i].schedule_date > d_date) {
                                    if ((it1->second->schedule_datum[0].schedule_date == it1->second->schedule_datum[temp_i].schedule_date) && (it1->second->GetDisburse_date() < d_date) && (d_date < it1->second->schedule_datum[0].schedule_date) && d_date == it1->second->GetLoan_fully_paid_date()) {
                                        temp_i = 0;
                                    } else {
                                        temp_i = temp_i - 1;
                                    }
                                    break;
                                    //temp_i = temp_i - 1;
                                    //break;
                                } else if (it1->second->schedule_datum[temp_i].schedule_date == d_date) {
                                    break;
                                }
                            }
                            if (temp_i == it1->second->getLastInstallmentNumber() || temp_i == it1->second->getFirstInstallmentNumber() - 1) {
                                continue;
                            }

                            flag = 1;
                            samity_idd = samity_list[i];
                            no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                            cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it1->second->schedule_datum[temp_i].installment_amount;

                            //zval *subarray2;
                            //MAKE_STD_ZVAL(subarray2);
                            //array_init(subarray2);

                            add_assoc_long(&subarray1, "loan_id", it1->second->GetId());
                            add_assoc_double(&subarray1, "loan_amount", it1->second->GetLoan_amount());
                            add_assoc_double(&subarray1, "total_payable_amount", it1->second->GetTotal_payable_amount());
                            add_assoc_long(&subarray1, "cycle", it1->second->GetCycle());                           
                            add_assoc_string(&subarray1, "loan_type",const_cast<char*> (it1->second->GetLoan_type().c_str()));
                            add_assoc_string(&subarray1, "repayment_frequency",const_cast<char*> (it1->second->GetRepayment_frequency().c_str()));
                            
                            add_assoc_long(&subarray1, "product_id", it1->second->GetProduct_id());
                            add_assoc_long(&subarray1, "installment_no", temp_i + 1);
                            add_assoc_double(&subarray1, "installment_amount", it1->second->schedule_datum[temp_i].installment_amount);
                            add_assoc_double(&subarray1, "principle_installment_amount", it1->second->schedule_datum[temp_i].principle_installment_amount);
                            add_assoc_double(&subarray1, "interrest_installment_amount", it1->second->schedule_datum[temp_i].interrest_installment_amount);


                            add_assoc_string(&subarray1, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[temp_i].schedule_date).c_str()));

                            add_assoc_long(&subarray1, "total_installment_no", it1->second->GetActualNumber_of_installment());
                            add_assoc_double(&subarray1, "last_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].installment_amount);
                            add_assoc_double(&subarray1, "last_principle_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].principle_installment_amount);
                            add_assoc_double(&subarray1, "last_interrest_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].interrest_installment_amount);

                            //add_assoc_string(subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                            std::string temp_last_date;
                            temp_last_date = this->get_last_schedule_date(it1->second->GetId());
                            boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                            add_assoc_string(&subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                            double current_principle_recoverable = 0;
                            double current_interrest_recoverable = 0;
                            double current_installment_recoverable = 0;
                            for (int i = it1->second->getFirstInstallmentNumber(); i < temp_i + 1; i++) {
                                current_principle_recoverable = current_principle_recoverable + it1->second->schedule_datum[i].principle_installment_amount;
                                current_interrest_recoverable = current_interrest_recoverable + it1->second->schedule_datum[i].interrest_installment_amount;
                                current_installment_recoverable = current_installment_recoverable + it1->second->schedule_datum[i].installment_amount;
                            }
                            add_assoc_double(&subarray1, "current_installment_recoverable", current_installment_recoverable);
                            add_assoc_double(&subarray1, "current_principle_recoverable", current_principle_recoverable);
                            add_assoc_double(&subarray1, "current_interrest_recoverable", current_interrest_recoverable);
                            //add_index_zval(subarray,temp_i+1,subarray1);
                            //add_index_zval(array1,samity_list[i],subarray);

                        }
                    }
                    //add_index_zval(array1,samity_list[i],subarray);

                    if (flag == 1) {
                        add_index_zval(&subarray, it1->second->GetId(), &subarray1);
                    }


                }
            }
            if (samity_idd) {
                add_assoc_long(&subarray, "samity_id", samity_idd);
                add_assoc_long(&subarray, "no_current_borrower", no_cumilative_expaired_borrower);
                add_assoc_double(&subarray, "cumilative_current_amount", cumilative_expaired_borrower_amount);
                cumilative_branch_loan_amount = cumilative_branch_loan_amount + cumilative_expaired_borrower_amount;
                add_index_zval(&array1, samity_list[i], &subarray);
            }
        }

    }
    add_assoc_double(&array1, "cumilative_branch_loan_amount", cumilative_branch_loan_amount);
    ////exit(1);
    return array1;
}

/////

zval Loan_scheduler::get_samity_wise_current_loan_information_just_before_date(std::vector<int> samity_list, std::string s_date, int branch_id, int product_id, int is_report) {

    boost::gregorian::date d_date(boost::gregorian::from_simple_string(s_date));

    //std::cerr << "In Just Before Date" << std::endl;
    //std::cerr << "Input:" << s_date << " " << branch_id << " " << product_id << std::endl;
    double cumilative_branch_loan_amount = 0;

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > >branch_samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<int, float> samity_recov;
    samity_recov samity_recovery;

    if (product_id && branch_id) {
        //std::cerr << "In Just Before Date: Branch,Product" << std::endl;
        for (int i = 0; i < samity_list.size(); i++) {

            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;
            for (loan_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]][product_id].end(); it++) {

                zval subarray1;
//                MAKE_STD_ZVAL(subarray1);
                array_init(&subarray1);

                int flag = 0;

                if (it->second->GetLoan_fully_paid_date() > d_date) {
                    //if(it->second.schedule_datum[0].schedule_date<d_date){
                    no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                    int temp_i = 0;
                    int schedule_index = -1;
                    for (temp_i = it->second->getFirstInstallmentNumber(); temp_i < it->second->getLastInstallmentNumber(); temp_i++) {
                        if (it->second->schedule_datum[temp_i].schedule_date > d_date) {
                            temp_i--;
                            break;
                        }
                        schedule_index = temp_i;
                    }
                    temp_i = schedule_index;
                    if (is_report == 0)
                        if (temp_i == it->second->getLastInstallmentNumber() - 1) {
                            if (it->second->schedule_datum[temp_i].schedule_date >= d_date)
                                temp_i = 0;

                        }
                    if (temp_i == it->second->getFirstInstallmentNumber() - 1) {
                        continue;
                    }
                    flag = 1;
                    cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it->second->schedule_datum[temp_i].installment_amount;
                    samity_idd = samity_list[i];


                    //zval *subarray2;
                    //MAKE_STD_ZVAL(subarray2);
                    //array_init(subarray2);

                    add_assoc_long(&subarray1, "loan_id", it->second->GetId());
                    add_assoc_long(&subarray1, "product_id", it->second->GetProduct_id());
                    add_assoc_long(&subarray1, "loan_amount", it->second->GetLoan_amount());
                    add_assoc_long(&subarray1, "member_id", it->second->GetMember_id());
                    add_assoc_long(&subarray1, "total_payable_amount", it->second->GetTotal_payable_amount());

                    add_assoc_long(&subarray1, "installment_no", temp_i + 1);
                    add_assoc_double(&subarray1, "installment_amount", it->second->schedule_datum[temp_i].installment_amount);
                    add_assoc_double(&subarray1, "principle_installment_amount", it->second->schedule_datum[temp_i].principle_installment_amount);
                    add_assoc_double(&subarray1, "interrest_installment_amount", it->second->schedule_datum[temp_i].interrest_installment_amount);


                    add_assoc_string(&subarray1, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[temp_i].schedule_date).c_str()));

                    add_assoc_long(&subarray1, "total_installment_no", it->second->GetActualNumber_of_installment());
                    add_assoc_double(&subarray1, "last_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].installment_amount);
                    add_assoc_double(&subarray1, "last_principle_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].principle_installment_amount);
                    add_assoc_double(&subarray1, "last_interrest_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].interrest_installment_amount);



                    //add_assoc_string(subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                    std::string temp_last_date;
                    temp_last_date = this->get_last_schedule_date(it->second->GetId());
                    boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                    add_assoc_string(&subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));


                    double current_principle_recoverable = 0;
                    double current_interrest_recoverable = 0;
                    double current_installment_recoverable = 0;
                    for (int i = it->second->getFirstInstallmentNumber(); i < temp_i + 1; i++) {
                        current_principle_recoverable = current_principle_recoverable + it->second->schedule_datum[i].principle_installment_amount;
                        current_interrest_recoverable = current_interrest_recoverable + it->second->schedule_datum[i].interrest_installment_amount;
                        current_installment_recoverable = current_installment_recoverable + it->second->schedule_datum[i].installment_amount;
                    }
                    add_assoc_double(&subarray1, "current_installment_recoverable", current_installment_recoverable);
                    add_assoc_double(&subarray1, "current_principle_recoverable", current_principle_recoverable);
                    add_assoc_double(&subarray1, "current_interrest_recoverable", current_interrest_recoverable);

                    //add_index_zval(subarray,temp_i+1,subarray1);
                    //add_index_zval(array1,samity_list[i],subarray);

                    //}
                }
                //add_index_zval(subarray3,samity_list[i],subarray);
                if (flag == 1) {
                    add_index_zval(&subarray, it->second->GetId(), &subarray1);
                }



            }
            if (samity_idd) {
                add_assoc_long(&subarray, "samity_id", samity_idd);
                add_assoc_long(&subarray, "no_current_borrower", no_cumilative_expaired_borrower);
                add_assoc_double(&subarray, "cumilative_current_amount", cumilative_expaired_borrower_amount);
                cumilative_branch_loan_amount = cumilative_branch_loan_amount + cumilative_expaired_borrower_amount;
                add_index_zval(&array1, samity_list[i], &subarray);
            }
        }
    } else if (branch_id) {
        //std::cerr << "In Just Before Date: Branch 1" << std::endl;
        for (int i = 0; i < samity_list.size(); i++) {
            //std::cerr << "In Just Before Date: Branch 2 " << samity_list[i] << std::endl;
            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;
            //for (branch_samity_loan_product_hash::iterator it2 = branch_samity_loan_product_list.begin(); it2 != branch_samity_loan_product_list.end(); it2++) {
            //std::cerr << "In Just Before Date: Branch 3" << std::endl;
            //for (loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]].end(); it++) {
            //std::cerr << "In Just Before Date: Branch 4 " << branch_id << " " << samity_list[i] << std::endl;
            //for (loan_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
            for (loan_hash::iterator it1 = branch_samity_loan_list[branch_id][samity_list[i]].begin(); it1 != branch_samity_loan_list[branch_id][samity_list[i]].end(); it1++) {
                //std::cerr << "In Just Before Date: Branch 5" << std::endl;
                int flag = 0;
                zval subarray1;
//                MAKE_STD_ZVAL(subarray1);
                array_init(&subarray1);



                if (it1->second->GetLoan_fully_paid_date() > d_date) {
                    //std::cerr << "In Just Before Date: Branch 6 "<<it1->second->GetId() << std::endl;
                    //if (it1->second->schedule_datum[0].schedule_date < d_date) {
                    //std::cerr << "In Just Before Date: Branch 7 " << d_date << std::endl;
                    int temp_i = 0;
                    int schedule_index = -1;
                    for (temp_i = it1->second->getFirstInstallmentNumber(); temp_i < it1->second->getLastInstallmentNumber(); temp_i++) {
                        if (it1->second->schedule_datum[temp_i].schedule_date > d_date) {
                            temp_i--;
                            break;
                        }
                        schedule_index = temp_i;
                    }
                    temp_i = schedule_index;
                    if (is_report == 0)
                        if (temp_i == it1->second->getLastInstallmentNumber() - 1) {
                            if (it1->second->schedule_datum[temp_i].schedule_date >= d_date)
                                temp_i = 0;

                        }
                    if (temp_i == it1->second->getFirstInstallmentNumber() - 1) {
                        continue;
                    }

                    flag = 1;
                    samity_idd = samity_list[i];
                    no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                    cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it1->second->schedule_datum[temp_i].installment_amount;

                    //zval *subarray2;
                    //MAKE_STD_ZVAL(subarray2);
                    //array_init(subarray2);

                    add_assoc_long(&subarray1, "loan_id", it1->second->GetId());
                    add_assoc_long(&subarray1, "product_id", it1->second->GetProduct_id());
                    add_assoc_long(&subarray1, "loan_amount", it1->second->GetLoan_amount());
                    add_assoc_long(&subarray1, "member_id", it1->second->GetMember_id());
                    add_assoc_long(&subarray1, "total_payable_amount", it1->second->GetTotal_payable_amount());
                    add_assoc_long(&subarray1, "installment_no", temp_i + 1);
                    add_assoc_double(&subarray1, "installment_amount", it1->second->schedule_datum[temp_i].installment_amount);
                    add_assoc_double(&subarray1, "principle_installment_amount", it1->second->schedule_datum[temp_i].principle_installment_amount);
                    add_assoc_double(&subarray1, "interrest_installment_amount", it1->second->schedule_datum[temp_i].interrest_installment_amount);

                    //std::cerr << "In Just Before Date: before date convert1 "<<it1->second->GetId()<<" " << temp_i << " " << it1->second->schedule_datum[temp_i].schedule_date << std::endl;
                    add_assoc_string(&subarray1, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[temp_i].schedule_date).c_str()));

                    add_assoc_long(&subarray1, "total_installment_no", it1->second->GetActualNumber_of_installment());
                    add_assoc_double(&subarray1, "last_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].installment_amount);
                    add_assoc_double(&subarray1, "last_principle_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].principle_installment_amount);
                    add_assoc_double(&subarray1, "last_interrest_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].interrest_installment_amount);

                    //std::cerr << "In Just Before Date: before date convert2" << std::endl;

                    //done for One time weekly ASM bcoz of number of installment 1 but behaviour regular
                    std::string temp_last_date;
                    temp_last_date = this->get_last_schedule_date(it1->second->GetId());
                    boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));

                    //add_assoc_string(subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                    add_assoc_string(&subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                    double current_principle_recoverable = 0;
                    double current_interrest_recoverable = 0;
                    double current_installment_recoverable = 0;
                    for (int i = it1->second->getFirstInstallmentNumber(); i < temp_i + 1; i++) {
                        current_principle_recoverable = current_principle_recoverable + it1->second->schedule_datum[i].principle_installment_amount;
                        current_interrest_recoverable = current_interrest_recoverable + it1->second->schedule_datum[i].interrest_installment_amount;
                        current_installment_recoverable = current_installment_recoverable + it1->second->schedule_datum[i].installment_amount;
                    }
                    add_assoc_double(&subarray1, "current_installment_recoverable", current_installment_recoverable);
                    add_assoc_double(&subarray1, "current_principle_recoverable", current_principle_recoverable);
                    add_assoc_double(&subarray1, "current_interrest_recoverable", current_interrest_recoverable);
                    //add_index_zval(subarray,temp_i+1,subarray1);
                    //add_index_zval(array1,samity_list[i],subarray);

                    //}
                }
                //add_index_zval(array1,samity_list[i],subarray);

                if (flag == 1) {
                    add_index_zval(&subarray, it1->second->GetId(), &subarray1);
                }


            }
            //}
            // }
            if (samity_idd) {
                add_assoc_long(&subarray, "samity_id", samity_idd);
                add_assoc_long(&subarray, "no_current_borrower", no_cumilative_expaired_borrower);
                add_assoc_double(&subarray, "cumilative_current_amount", cumilative_expaired_borrower_amount);
                cumilative_branch_loan_amount = cumilative_branch_loan_amount + cumilative_expaired_borrower_amount;
                add_index_zval(&array1, samity_list[i], &subarray);
            }
        }

    } else {
        //std::cerr << "In Just Before Date: Empty" << std::endl;
        for (int i = 0; i < samity_list.size(); i++) {

            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;
            for (loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]].end(); it++) {

                for (loan_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
                    int flag = 0;
                    zval subarray1;
//                    MAKE_STD_ZVAL(subarray1);
                    array_init(&subarray1);



                    if (it1->second->GetLoan_fully_paid_date() > d_date) {
                        if (it1->second->schedule_datum[0].schedule_date < d_date) {
                            int temp_i = 0;
                            int schedule_index = -1;
                            for (temp_i = it1->second->getFirstInstallmentNumber(); temp_i < it1->second->getFirstInstallmentNumber(); temp_i++) {
                                if (it1->second->schedule_datum[temp_i].schedule_date > d_date) {
                                    temp_i--;
                                    break;
                                }
                                schedule_index = temp_i;
                            }
                            temp_i = schedule_index;
                            if (is_report == 0)
                                if (temp_i == it1->second->getFirstInstallmentNumber() - 1) {
                                    if (it1->second->schedule_datum[temp_i].schedule_date >= d_date)
                                        temp_i = 0;

                                }
                            if (temp_i == it1->second->getLastInstallmentNumber() - 1) {
                                continue;
                            }

                            flag = 1;
                            samity_idd = samity_list[i];
                            no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                            cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it1->second->schedule_datum[temp_i].installment_amount;

                            //zval *subarray2;
                            //MAKE_STD_ZVAL(subarray2);
                            //array_init(subarray2);

                            add_assoc_long(&subarray1, "loan_id", it1->second->GetId());
                            add_assoc_long(&subarray1, "product_id", it1->second->GetProduct_id());
                            add_assoc_long(&subarray1, "installment_no", temp_i + 1);
                            add_assoc_double(&subarray1, "installment_amount", it1->second->schedule_datum[temp_i].installment_amount);
                            add_assoc_double(&subarray1, "principle_installment_amount", it1->second->schedule_datum[temp_i].principle_installment_amount);
                            add_assoc_double(&subarray1, "interrest_installment_amount", it1->second->schedule_datum[temp_i].interrest_installment_amount);


                            add_assoc_string(&subarray1, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[temp_i].schedule_date).c_str()));

                            add_assoc_long(&subarray1, "total_installment_no", it1->second->GetActualNumber_of_installment());
                            add_assoc_double(&subarray1, "last_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].installment_amount);
                            add_assoc_double(&subarray1, "last_principle_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].principle_installment_amount);
                            add_assoc_double(&subarray1, "last_interrest_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].interrest_installment_amount);

                            //add_assoc_string(subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                            std::string temp_last_date;
                            temp_last_date = this->get_last_schedule_date(it1->second->GetId());
                            boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                            add_assoc_string(&subarray1, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));


                            double current_principle_recoverable = 0;
                            double current_interrest_recoverable = 0;
                            double current_installment_recoverable = 0;
                            for (int i = it1->second->getFirstInstallmentNumber(); i < temp_i + 1; i++) {
                                current_principle_recoverable = current_principle_recoverable + it1->second->schedule_datum[i].principle_installment_amount;
                                current_interrest_recoverable = current_interrest_recoverable + it1->second->schedule_datum[i].interrest_installment_amount;
                                current_installment_recoverable = current_installment_recoverable + it1->second->schedule_datum[i].installment_amount;
                            }
                            add_assoc_double(&subarray1, "current_installment_recoverable", current_installment_recoverable);
                            add_assoc_double(&subarray1, "current_principle_recoverable", current_principle_recoverable);
                            add_assoc_double(&subarray1, "current_interrest_recoverable", current_interrest_recoverable);
                            //add_index_zval(subarray,temp_i+1,subarray1);
                            //add_index_zval(array1,samity_list[i],subarray);

                        }
                    }
                    //add_index_zval(array1,samity_list[i],subarray);

                    if (flag == 1) {
                        add_index_zval(&subarray, it1->second->GetId(), &subarray1);
                    }


                }
            }
            if (samity_idd) {
                add_assoc_long(&subarray, "samity_id", samity_idd);
                add_assoc_long(&subarray, "no_current_borrower", no_cumilative_expaired_borrower);
                add_assoc_double(&subarray, "cumilative_current_amount", cumilative_expaired_borrower_amount);
                cumilative_branch_loan_amount = cumilative_branch_loan_amount + cumilative_expaired_borrower_amount;
                add_index_zval(&array1, samity_list[i], &subarray);
            }
        }

    }
    add_assoc_double(&array1, "cumilative_branch_loan_amount", cumilative_branch_loan_amount);
    return array1;
}

zval Loan_scheduler::get_samity_wise_current_loan_information_between_date(std::vector<int> samity_list, std::string s_date_from, std::string s_date_to, int branch_id, int product_id) {

    //std::cerr << "In Curent Loan Between Date" << std::endl;
    //std::cerr << "Input:" << s_date_from << " " << s_date_to << " " << branch_id << " " << product_id << std::endl;

    boost::gregorian::date d_date_from(boost::gregorian::from_simple_string(s_date_from));
    boost::gregorian::date d_date_to(boost::gregorian::from_simple_string(s_date_to));



    double cumilative_branch_loan_amount = 0;

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<int, float> samity_recov;
    samity_recov samity_recovery;

    if (product_id) {
        for (int i = 0; i < samity_list.size(); i++) {

            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;
            for (loan_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]][product_id].end(); it++) {

                zval subarray1;
//                MAKE_STD_ZVAL(subarray1);
                array_init(&subarray1);

                int flag = 0;

                if (it->second->GetLoan_fully_paid_date() >= d_date_from) {
                    // if (it->second->schedule_datum[0].schedule_date > d_date_from && it->second->schedule_datum[0].schedule_date < d_date_to)
                    {
                        no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                        int temp_i = 0;
                        for (temp_i = it->second->getFirstInstallmentNumber(); temp_i < it->second->getLastInstallmentNumber(); temp_i++) {
                            if (it->second->schedule_datum[temp_i].schedule_date >= d_date_from && it->second->schedule_datum[temp_i].schedule_date <= d_date_to) {

                                flag = 1;
                                cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it->second->schedule_datum[temp_i].installment_amount;
                                samity_idd = samity_list[i];

                                zval subarray2;
//                                MAKE_STD_ZVAL(subarray2);
                                array_init(&subarray2);

                                add_assoc_long(&subarray2, "loan_id", it->second->GetId());
                                add_assoc_double(&subarray1, "loan_amount", it->second->GetLoan_amount());
                                add_assoc_double(&subarray1, "total_payable_amount", it->second->GetTotal_payable_amount());

                                add_assoc_long(&subarray2, "installment_no", temp_i + 1);
                                add_assoc_double(&subarray2, "installment_amount", it->second->schedule_datum[temp_i].installment_amount);
                                add_assoc_double(&subarray2, "principle_installment_amount", it->second->schedule_datum[temp_i].principle_installment_amount);
                                add_assoc_string(&subarray2, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[temp_i].schedule_date).c_str()));

                                add_assoc_long(&subarray2, "total_installment_no", it->second->GetActualNumber_of_installment());
                                add_assoc_double(&subarray2, "last_installment_amount", it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].installment_amount);

                                // add_assoc_string(subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                                std::string temp_last_date;
                                temp_last_date = this->get_last_schedule_date(it->second->GetId());
                                boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                                add_assoc_string(&subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                                add_index_zval(&subarray1, temp_i + 1, &subarray2);
                                //add_index_zval(array1,samity_list[i],subarray);
                            }
                        }
                    }
                }
                //add_index_zval(subarray3,samity_list[i],subarray);
                if (flag == 1) {
                    add_index_zval(&subarray, it->second->GetId(), &subarray1);
                }



            }
            if (samity_idd) {
                add_assoc_long(&subarray, "samity_id", samity_idd);
                add_assoc_long(&subarray, "no_current_borrower", no_cumilative_expaired_borrower);
                add_assoc_double(&subarray, "cumilative_current_amount", cumilative_expaired_borrower_amount);
                cumilative_branch_loan_amount = cumilative_branch_loan_amount + cumilative_expaired_borrower_amount;
                add_index_zval(&array1, samity_list[i], &subarray);
            }
        }
    } else {
        for (int i = 0; i < samity_list.size(); i++) {

            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;
            for (loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]].end(); it++) {

                for (loan_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
                    int flag = 0;
                    zval subarray1;
//                    MAKE_STD_ZVAL(subarray1);
                    array_init(&subarray1);



                    if (it1->second->GetLoan_fully_paid_date() >= d_date_from) {
                        //if (it1->second->schedule_datum[0].schedule_date > d_date_from && it1->second->schedule_datum[0].schedule_date < d_date_to)
                        {
                            int temp_i = 0;
                            for (temp_i = it1->second->getFirstInstallmentNumber(); temp_i < it1->second->getLastInstallmentNumber(); temp_i++) {
                                if (it1->second->schedule_datum[temp_i].schedule_date >= d_date_from && it1->second->schedule_datum[temp_i].schedule_date <= d_date_to) {

                                    flag = 1;
                                    samity_idd = samity_list[i];
                                    no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                                    cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it1->second->schedule_datum[temp_i].installment_amount;

                                    zval subarray2;
//                                    MAKE_STD_ZVAL(subarray2);
                                    array_init(&subarray2);

                                    add_assoc_long(&subarray2, "loan_id", it1->second->GetId());
                                    add_assoc_long(&subarray2, "installment_no", temp_i + 1);
                                    add_assoc_double(&subarray1, "loan_amount", it1->second->GetLoan_amount());
                                    add_assoc_double(&subarray1, "total_payable_amount", it1->second->GetTotal_payable_amount());

                                    add_assoc_double(&subarray2, "installment_amount", it1->second->schedule_datum[temp_i].installment_amount);
                                    add_assoc_double(&subarray2, "principle_installment_amount", it1->second->schedule_datum[temp_i].principle_installment_amount);
                                    add_assoc_string(&subarray2, "repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[temp_i].schedule_date).c_str()));

                                    add_assoc_long(&subarray2, "total_installment_no", it1->second->GetActualNumber_of_installment());
                                    add_assoc_double(&subarray2, "last_installment_amount", it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].installment_amount);

                                    //add_assoc_string(subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].schedule_date).c_str()), 1);
                                    std::string temp_last_date;
                                    temp_last_date = this->get_last_schedule_date(it1->second->GetId());
                                    boost::gregorian::date temp_last_date_cus(boost::gregorian::from_simple_string(temp_last_date));
                                    add_assoc_string(&subarray2, "last_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(temp_last_date_cus).c_str()));

                                    add_index_zval(&subarray1, temp_i + 1, &subarray2);
                                    //add_index_zval(array1,samity_list[i],subarray);
                                }
                            }
                        }
                    }
                    //add_index_zval(array1,samity_list[i],subarray);

                    if (flag == 1) {
                        add_index_zval(&subarray, it1->second->GetId(), &subarray1);
                    }


                }
            }
            if (samity_idd) {
                add_assoc_long(&subarray, "samity_id", samity_idd);
                add_assoc_long(&subarray, "no_current_borrower", no_cumilative_expaired_borrower);
                add_assoc_double(&subarray, "cumilative_current_amount", cumilative_expaired_borrower_amount);
                cumilative_branch_loan_amount = cumilative_branch_loan_amount + cumilative_expaired_borrower_amount;
                add_index_zval(&array1, samity_list[i], &subarray);
            }
        }

    }
    add_assoc_double(&array1, "cumilative_branch_loan_amount", cumilative_branch_loan_amount);
    return array1;
}

void Loan_scheduler::load_samity_day_list(samity_hash samity_list) {
    this->samity_list = samity_list;
}

void Loan_scheduler::load_holiday_list(br_hash* branch_holiday_list, samity_date_hash samity_holiday_list) {
    this->branch_holiday_list_p = branch_holiday_list;
    this->samity_holiday_list = samity_holiday_list;
}

void Loan_scheduler::load_loan_list(loan_hash loan_list) {
    this->loan_list = loan_list;
}

void Loan_scheduler::load_samity_day_change(date_hash samity_day_change_list) {
    this->samity_day_change_list = samity_day_change_list;
}

void Loan_scheduler::get_valid_schedule_date(boost::gregorian::date& d_date, int samity_id, int loan_id) {

    /*
        if (d_date.day_of_week() != this->samity_list[samity_id].GetSamity_week_day(d_date)) {
            d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id].GetSamity_week_day(d_date));
        }
     */
    //std::cerr<<d_date<<" "<<samity_id<<" "<<loan_id<<std::endl;
    int flag = 0;
    for (std::vector<date_pair>::iterator it = loan_reschedule_list[loan_id].begin(); it != loan_reschedule_list[loan_id].end(); it++) {
        if (d_date >= it->start && d_date <= it->end) {
            d_date = it->end; //+boost::gregorian::days(1);
            flag = 1;
            break;
        }
    }

    //std::cerr<<d_date<<" "<<samity_id<<" "<<loan_id<<std::endl;
    //std::string s_date = boost::gregorian::to_iso_extended_string(d_date);
    while ((this->branch_holiday_list_p->find(d_date) != this->branch_holiday_list_p->end()) || (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end())) {

        //d_date = d_date + boost::gregorian::days(7);
        //d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id].GetSamity_week_day(d_date));
        d_date = d_date + boost::gregorian::days(1);
        flag = 1;
        //s_date = boost::gregorian::to_iso_extended_string(d_date);
    }

    //std::cerr<<d_date<<" "<<samity_id<<" "<<loan_id<<std::endl;
    /* Samity holiday condition marged with the branch holiday loop above
    if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end())
        while (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end()) {

            //d_date = d_date + boost::gregorian::days(7);
            //d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id].GetSamity_week_day(d_date));
            d_date = d_date + boost::gregorian::days(1);
            flag = 1;
            //s_date = boost::gregorian::to_iso_extended_string(d_date);
        }

	*/
    //if(flag==1){
    d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));

    //}
    //return d_date;

}

void Loan_scheduler::get_daily_valid_schedule_date(boost::gregorian::date& d_date, int samity_id, int loan_id) {

    d_date = d_date + boost::gregorian::days(1);
    //std::cout<<"After "<<d_date<<std::endl;
    //std::cerr<<"Date: "<<d_date<<std::endl;
    for (it = loan_reschedule_list[loan_id].begin(); it != loan_reschedule_list[loan_id].end(); it++) {
        if (d_date >= it->start && d_date <= it->end) {
            d_date = it->end; //+ boost::gregorian::days(1);
            break;
        }
    }

    //std::string s_date = boost::gregorian::to_iso_extended_string(d_date);
    while ((this->branch_holiday_list_p->find(d_date) != this->branch_holiday_list_p->end()) || (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end())) {

        //d_date = d_date + boost::gregorian::days(7);
        //d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id].GetSamity_week_day(d_date));
        d_date = d_date + boost::gregorian::days(1);
        //s_date = boost::gregorian::to_iso_extended_string(d_date);
    }
    /* Samity holiday condition marged with the branch holiday loop above
    if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end())
        while (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end()) {

            //d_date = d_date + boost::gregorian::days(7);
            //d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id].GetSamity_week_day(d_date));
            d_date = d_date + boost::gregorian::days(1);
            //s_date = boost::gregorian::to_iso_extended_string(d_date);
        }*/
    /*
     */

    //std::cout<<"After "<<d_date<<" "<<this->samity_list[samity_id].GetSamity_week_day(boost::gregorian::to_iso_extended_string(d_date))<<std::endl;
    //std::cout<<"After "<<d_date<<std::endl;
    //return d_date;

}

void Loan_scheduler::get_weekly_valid_schedule_date_samity_day(boost::gregorian::date& d_date, int samity_id, int loan_id) {
    d_date = d_date + boost::gregorian::weeks(1);
    for (it = loan_reschedule_list[loan_id].begin(); it != loan_reschedule_list[loan_id].end(); it++) {
        if (d_date >= it->start && d_date <= it->end) {
            d_date = it->end; //+ boost::gregorian::days(1);
            break;
        }
    }
    while ((this->branch_holiday_list_p->find(d_date) != this->branch_holiday_list_p->end()) || (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end())) {
        d_date = d_date + boost::gregorian::weeks(1);
    }
    /* Samity holiday condition marged with the branch holiday loop above
    if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end()) {
        while (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end()) {
            d_date = d_date + boost::gregorian::weeks(1);
        }
    }*/
    d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
}

void Loan_scheduler::get_weekly_valid_schedule_date(boost::gregorian::date& d_date, int samity_id, int loan_id) {
    d_date = d_date + boost::gregorian::weeks(1);
    for (it = loan_reschedule_list[loan_id].begin(); it != loan_reschedule_list[loan_id].end(); it++) {
        if (d_date >= it->start && d_date <= it->end) {
            d_date = it->end; //+ boost::gregorian::days(1);
            break;
        }
    }
    while ((this->branch_holiday_list_p->find(d_date) != this->branch_holiday_list_p->end()) || (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end())) {
        d_date = d_date + boost::gregorian::weeks(1);
    }
    /* Samity holiday condition marged with the branch holiday loop above
    if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end()) {
        while (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end()) {
            d_date = d_date + boost::gregorian::weeks(1);
        }
    }*/
    //d_date=boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
}

int Loan_scheduler::get_loan_week_number_considering_date(boost::gregorian::date d_date, int samity_id, int loan) {

    boost::gregorian::date::ymd_type ymd = d_date.year_month_day();
    boost::gregorian::date tmp_d_date(ymd.year, ymd.month, 1);

    int month_week = 0;
    while (tmp_d_date <= d_date) {
        if (this->samity_list[samity_id]->GetSamity_week_day(d_date) == tmp_d_date.day_of_week()) {
            month_week++;
        }
        tmp_d_date = tmp_d_date + boost::gregorian::days(1);
    }
    return month_week;
}

void Loan_scheduler::get_monthly_valid_schedule_date_consider_date(boost::gregorian::date& d_date, int samity_id, int loan_id, int ins_no, int month_date, int month_week,bool is_first_repayment_date) {

    if(is_first_repayment_date){
    for (int i = 0; i < ins_no; i++) {
        d_date = d_date + boost::gregorian::months(1);
        for (it = loan_reschedule_list[loan_id].begin(); it != loan_reschedule_list[loan_id].end(); it++) {
            boost::gregorian::date d_date1 = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
            boost::gregorian::date::ymd_type ymd1 = d_date.year_month_day();
            boost::gregorian::date::ymd_type ymd2 = it->start.year_month_day();
            if (d_date1 == it->start || (ymd1.year == ymd2.year && ymd1.month == ymd2.month)) {
                d_date = it->end;
                break;
            }
        }
    }
    }else{
        d_date = d_date + boost::gregorian::months(1);
    }

    //std::cerr<<"month_date "<<month_date<<" month_week "<<month_week<<" d_date day "<<d_date.day()<<"\n";
//    std::cerr << "ins " << ins_no << " month "<<month_date << " month_week " << month_week <<std::endl;
    if (month_date > 0) {
        boost::gregorian::date::ymd_type ymd = d_date.year_month_day();
        boost::gregorian::date tmp_d_date(ymd.year, ymd.month, month_date);
        d_date = tmp_d_date;
    } else if (month_week >= 1 && month_week <= 4) {
        boost::gregorian::date::ymd_type ymd = d_date.year_month_day();
        boost::gregorian::date tmp_d_date(ymd.year, ymd.month, 1);
        d_date = tmp_d_date;
        d_date = d_date - boost::gregorian::days(1);
        while (month_week > 0) {
            d_date = d_date + boost::gregorian::days(1);
            if (this->samity_list[samity_id]->GetSamity_week_day(d_date) == d_date.day_of_week()) {
                month_week--;
            }
        }
    } else if (month_week > 4) {
        boost::gregorian::date::ymd_type ymd = d_date.year_month_day();
        boost::gregorian::date tmp_d_date = d_date.end_of_month();
        d_date = tmp_d_date;
        d_date = boost::gregorian::previous_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));

    } else if (month_week == -1 && this->loan_list[loan_id]->Get_skip_last_week_calculation_monthly() == 0) {
        boost::gregorian::date::ymd_type ymd = d_date.year_month_day();
        boost::gregorian::date tmp_date = d_date.end_of_month();
        d_date = tmp_date + boost::gregorian::days(-6);
    }

    d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
    // For ignoring of the same month schedule installment appear
    if(ins_no>0){
        boost::gregorian::date previous_schedule_date = this->loan_list[loan_id]->getSchedule_by_installment_no(ins_no-1);
		if(previous_schedule_date.month()==d_date.month()){
			d_date = d_date + boost::gregorian::months(1);
		}else if(previous_schedule_date.month()>d_date.month() || previous_schedule_date.year()>d_date.year()){
			while(previous_schedule_date.year()>d_date.year()){
					d_date = d_date + boost::gregorian::months(1);
			}
			while(previous_schedule_date.month()>=d_date.month() && previous_schedule_date.year()==d_date.year()){
				d_date = d_date + boost::gregorian::months(1);
			}
		}
    }


    if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end()) {
        while ((this->branch_holiday_list_p->find(d_date) != this->branch_holiday_list_p->end()) || (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end())) {
            d_date = d_date + boost::gregorian::days(1);
            d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
//            std::cerr <<ins_no << "#d_date "<< d_date <<std::endl;

        }
    } else {
        while ((this->branch_holiday_list_p->find(d_date) != this->branch_holiday_list_p->end())) {
            d_date = d_date + boost::gregorian::days(1);
            d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
//           std::cerr <<ins_no << "##d_date "<< d_date <<std::endl;
        }
    }
}

/* For prepare date considering holiday
 * @Author  : Nur Hossen
 * @CreationDate: 2021-02-18
 * @uses    : Loan_scheduler.cpp
 * @param   : int samity_id, date d_date, bool is_increment
 * @return  : date
 */
boost::gregorian::date Loan_scheduler::get_date_considering_holiday(int samity_id,boost::gregorian::date d_date,bool is_increment){
    if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end()) {
        while ((this->branch_holiday_list_p->find(d_date) != this->branch_holiday_list_p->end()) || (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end())) {
            if(is_increment){
                d_date = d_date + boost::gregorian::days(1);
            }else{
                d_date = d_date - boost::gregorian::days(1);
            }
        }
    } else {
        while ((this->branch_holiday_list_p->find(d_date) != this->branch_holiday_list_p->end())) {
            if(is_increment){
                d_date = d_date + boost::gregorian::days(1);
            }else{
                d_date = d_date - boost::gregorian::days(1);
            }
        }
    }
    
    return d_date;
}

// For specific date regards first repayment date
void Loan_scheduler::get_monthly_valid_schedule_date_consider_specific_date(boost::gregorian::date& d_date, int samity_id, int loan_id, int ins_no, int month_date, int month_week) {
    
    boost::gregorian::date temp_first_repayment_date = d_date;
    for (int i = 0; i < ins_no; i++) {
        d_date = d_date + boost::gregorian::months(1);
    }
    if (d_date.day() != temp_first_repayment_date.day()) {

        std::string yyyymmdd = boost::gregorian::to_iso_string(d_date);
        std::string ret = yyyymmdd.substr(4, 2) + "/" + yyyymmdd.substr(6, 2) + "/" + yyyymmdd.substr(0, 4);
        std::string month_in_string = yyyymmdd.substr(4, 2);
        std::string day_in_string = yyyymmdd.substr(6, 2);

        std::string temp_first_repayment_date_in_string = boost::gregorian::to_iso_string(temp_first_repayment_date);
        std::string temp_month_in_string = temp_first_repayment_date_in_string.substr(4, 2);
        std::string temp_day_in_string = temp_first_repayment_date_in_string.substr(6, 2);

        if (month_in_string == "02" && day_in_string >= "28") {
            boost::gregorian::date::ymd_type ymd = d_date.year_month_day();
            boost::gregorian::date tmp_d_date(ymd.year, ymd.month, 28);
            d_date = tmp_d_date;
        } else if ((month_in_string == "04" || month_in_string == "06" || month_in_string == "09" || month_in_string == "11") && temp_day_in_string >= "30") {
            boost::gregorian::date::ymd_type ymd = d_date.year_month_day();
            boost::gregorian::date tmp_d_date(ymd.year, ymd.month, 30);
            d_date = tmp_d_date;
        } else {
            boost::gregorian::date::ymd_type ymd = d_date.year_month_day();
            boost::gregorian::date tmp_d_date(ymd.year, ymd.month, temp_first_repayment_date.day());
            d_date = tmp_d_date;
        }

    }
    
    for (it = loan_reschedule_list[loan_id].begin(); it != loan_reschedule_list[loan_id].end(); it++) {
        if (d_date >= it->start && d_date <= it->end) {
            d_date = it->end;
            d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
            break;
        } else if (d_date == it->start) {
            d_date = it->end;
            break;
        }
    }
    // For ignoring of the same month schedule installment appear
     if(ins_no>0){
        boost::gregorian::date previous_schedule_date = this->loan_list[loan_id]->getSchedule_by_installment_no(ins_no-1);
		if(previous_schedule_date.month()==d_date.month()){
			d_date = d_date + boost::gregorian::months(1);
		}else if(previous_schedule_date.month()>d_date.month() || previous_schedule_date.year()>d_date.year()){
			while(previous_schedule_date.year()>d_date.year()){
					d_date = d_date + boost::gregorian::months(1);
			}
			while(previous_schedule_date.month()>=d_date.month() && previous_schedule_date.year()==d_date.year()){
				d_date = d_date + boost::gregorian::months(1);
			}
		}
    } 
    
    boost::gregorian::date::ymd_type ymd = d_date.year_month_day();
    boost::gregorian::date tmp_d_date = d_date.end_of_month();
    boost::gregorian::date tmp_d_date_actual = d_date;
    
    //Holiday check, param: samity_id, date, increment or decrement
    d_date = this->get_date_considering_holiday(samity_id,d_date,true);
  
    boost::gregorian::date::ymd_type ymd_d_date = d_date.year_month_day();
    boost::gregorian::date::ymd_type ymd_tmp_d_date_actual = tmp_d_date_actual.year_month_day();
    boost::gregorian::date ymd_tmp_d_date_actual1 = d_date;
    if (ymd_d_date.month != ymd_tmp_d_date_actual.month && ymd_d_date.year==ymd_tmp_d_date_actual.year) {
         //Holiday check, param: samity_id, date, increment or decrement
        d_date = tmp_d_date_actual = this->get_date_considering_holiday(samity_id,tmp_d_date_actual,false);       
    }
    ymd_d_date = d_date.year_month_day();
    if (ymd_d_date.month != ymd_tmp_d_date_actual.month && ymd_d_date.year==ymd_tmp_d_date_actual.year) {
        d_date=ymd_tmp_d_date_actual1;
    }
    
}



void Loan_scheduler::get_monthly_valid_schedule_date(boost::gregorian::date& d_date, int samity_id, int loan_id, int ins_no) {

    for (int i = 0; i < ins_no; i++) {
        d_date = d_date + boost::gregorian::months(1);
    }
    //std::cerr << d_date << std::endl;


    boost::gregorian::date sc_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
    boost::gregorian::date sc_d_date = d_date;
    while (sc_date.month() > d_date.month() || sc_date.year() > d_date.year()) {
        sc_d_date = sc_d_date - boost::gregorian::weeks(1);
        sc_date = boost::gregorian::next_weekday(sc_d_date, this->samity_list[samity_id]->GetSamity_week_day(sc_d_date));
    }
    d_date = sc_date;
    //d_date = d_date+boost::gregorian::weeks(ins_no*4);

    for (it = loan_reschedule_list[loan_id].begin(); it != loan_reschedule_list[loan_id].end(); it++) {
        if (d_date >= it->start && d_date <= it->end) {
            d_date = it->end; //+ boost::gregorian::days(1);
            d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
            break;
        }
    }

    //std::string s_date = boost::gregorian::to_iso_extended_string(d_date);
    while ((this->branch_holiday_list_p->find(d_date) != this->branch_holiday_list_p->end()) || (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end())) {
        d_date = d_date + boost::gregorian::days(1);
        d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
    }
    /* Samity holiday condition marged with the branch holiday loop above
    if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end()) {
        while (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end()) {
            d_date = d_date + boost::gregorian::days(1);
            d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
        }
    }*/
}

void Loan_scheduler::get_grace_period_wise_valid_schedule_date(boost::gregorian::date& d_date, int samity_id, int loan_id, int ins_no, int grace_period) {
    //std::cerr <<"Before "<< d_date << std::endl;
    for (int i = 0; i < ins_no; i++) {
        d_date = d_date + boost::gregorian::days(grace_period);
    }
    boost::gregorian::date sc_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
    boost::gregorian::date sc_d_date = d_date;
    d_date = sc_date;

    for (it = loan_reschedule_list[loan_id].begin(); it != loan_reschedule_list[loan_id].end(); it++) {
        if (d_date >= it->start && d_date <= it->end) {
            d_date = it->end; //+ boost::gregorian::days(1);
            d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
            break;
        }
    }

    //std::string s_date = boost::gregorian::to_iso_extended_string(d_date);
    while ((this->branch_holiday_list_p->find(d_date) != this->branch_holiday_list_p->end()) || (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end())) {
        d_date = d_date + boost::gregorian::days(1);
        d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
    }
    /*
    if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end())
        while (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end()) {
            d_date = d_date + boost::gregorian::days(1);
            d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
        }*/
}

void Loan_scheduler::get_monthly_valid_schedule_date_without_samityday(boost::gregorian::date& d_date, int samity_id, int loan_id, int ins_no) {

    for (int i = 0; i < ins_no; i++) {
        d_date = d_date + boost::gregorian::months(1);
    }

    boost::gregorian::date sc_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
    boost::gregorian::date sc_d_date = d_date;
    while (sc_date.month() > d_date.month() || sc_date.year() > d_date.year()) {
        sc_d_date = sc_d_date - boost::gregorian::weeks(1);
        sc_date = boost::gregorian::next_weekday(sc_d_date, this->samity_list[samity_id]->GetSamity_week_day(sc_d_date));
    }
    d_date = sc_date;

    for (it = loan_reschedule_list[loan_id].begin(); it != loan_reschedule_list[loan_id].end(); it++) {
        if (d_date >= it->start && d_date <= it->end) {
            d_date = it->end;
            d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
            break;
        }
    }

    while ((this->branch_holiday_list_p->find(d_date) != this->branch_holiday_list_p->end()) || (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end())) {
        d_date = d_date + boost::gregorian::days(1);
    }
    /*
    if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end())
        while (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end()) {
            d_date = d_date + boost::gregorian::days(1);
        }*/
}

void Loan_scheduler::get_yearly_valid_schedule_date(boost::gregorian::date& d_date, int samity_id, int loan_id) {


    d_date = d_date + boost::gregorian::years(1);
    d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));

    for (it = loan_reschedule_list[loan_id].begin(); it != loan_reschedule_list[loan_id].end(); it++) {
        if (d_date >= it->start && d_date <= it->end) {
            d_date = it->end; //+ boost::gregorian::days(1);
            d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
            break;
        }
    }

    //std::string s_date = boost::gregorian::to_iso_extended_string(d_date);
    while ((this->branch_holiday_list_p->find(d_date) != this->branch_holiday_list_p->end()) || (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end())) {
        d_date = d_date + boost::gregorian::days(1);
        d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
    }
    /*
    if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end()) {
        while (this->samity_holiday_list[samity_id].find(d_date) != this->samity_holiday_list[samity_id].end()) {
            d_date = d_date + boost::gregorian::days(1);
            d_date = boost::gregorian::next_weekday(d_date, this->samity_list[samity_id]->GetSamity_week_day(d_date));
        }
    }*/
}

void Loan_scheduler::get_next_schedule_date(boost::gregorian::date& d_date, int samity_id, std::string repayment_frequency, int loan_id) {
    this->get_valid_schedule_date(d_date, samity_id, loan_id);
}

void Loan_scheduler::run_scheduler(loan *ln) {
    // std::cerr<<"running scheduler"<<ln->GetRepayment_frequency()<<"\n";
    int count = 1;
    //date_hash schedule;
    bool is_rescheduled = false;
    boost::gregorian::date schedule_date = ln->GetFirst_repayment_date();
    
    //this->get_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());
   
    double payable_amount = 0;

    int samity_id = ln->GetSamity_id();
    int loan_id = ln->GetId();
    int no_of_ins = ln->GetActualNumber_of_installment();

    if (ln->GetLoan_type().compare("O") == 0) {
        ln->SetInterest_calculation_method("F");
        if (ln->GetRepayment_frequency().compare("O") == 0) {
            if (ln->GetNumber_of_installment() > 1) {
                boost::gregorian::date first_schedule_date = schedule_date;
                int monthgap = 6;
                monthgap = round(ln->GetLoan_period_in_month() / ln->GetNumber_of_installment());
                while (count <= no_of_ins) {
                    //ln->addNewFLATSchedule_date(schedule_date);
                    ln->addNewOneTimeSchedule_date_multiple(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), monthgap * count);
                    count += 1;
                }

            } else {
                ln->addNewOneTimeSchedule_date(schedule_date);
            }

        } else if (ln->GetRepayment_frequency().compare("M") == 0) {

            schedule_date = ln->GetDisburse_date() + boost::gregorian::months(1);

            this->get_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());

            samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
            samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;

            std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];

            boost::gregorian::date last_date = ln->GetFirst_repayment_date();
            int loan_period_in_month = ln->GetLoan_period_in_month();
            boost::gregorian::date first_repay_date_murad = ln->GetFirst_repayment_date();
            boost::gregorian::date pre_schedule_date = schedule_date;

            ln->SetFirst_repayment_date(last_date);
            if ((*this->system_date) < ln->GetFirst_repayment_date()) {
                last_date = (*this->system_date);
            }

            if (reschedule_list.empty())
                while (schedule_date <= last_date) {
                    pre_schedule_date = schedule_date;
                    //std::cerr<<" MURAD empty S_DATE count " <<count<< " : " <<schedule_date<<" last_date "<<last_date<<std::endl;
                    ln->addNewOneTimeFLATSchedule_dateMonthly(schedule_date);
                    is_rescheduled = false;
                    //schedule_date = (*samity_schedules_index)[week_count];
                    schedule_date = ln->GetDisburse_date() + boost::gregorian::months(1);
                    //this->get_weekly_valid_schedule_date(schedule_date,ln->GetSamity_id(),ln->GetId());
                    this->get_monthly_valid_schedule_date_without_samityday(schedule_date, ln->GetSamity_id(), ln->GetId(), count);
                    // std::cerr<<" MURAD empty after S_DATE count " <<count<< " : " <<schedule_date<<" last_date "<<last_date<<std::endl;
                    count += 1;
                    if (loan_period_in_month == count) {
                        break;
                    }
                    //week_count++; 
                }

            if (pre_schedule_date != last_date) {
                ln->addNewOneTimeFLATSchedule_dateMonthly(last_date, false);
            }


            if (ln->is_show_rec_after_loan_period == 1) {
                last_date = ln->GetFirst_repayment_date(false);
                if ((*this->system_date) > ln->GetFirst_repayment_date(false)) {
                    schedule_date = (*this->system_date);
                    ln->addNewOneTimeFLATSchedule_dateMonthly(schedule_date);
                    last_date = schedule_date;
                }
                ln->addLastOneTimeFLATSchedule_dateMonthly(last_date);

            } else {
                last_date = ln->GetFirst_repayment_date(false);
                ln->addLastOneTimeFLATSchedule_dateMonthly(last_date);
            }

            ln->SetLast_repayment_date(schedule_date);
            //if((*this->system_date)>ln->GetFirst_repayment_date()){
            //schedule_date=(*this->system_date);    
            //ln->addNewOneTimeFLATSchedule_dateMonthly(schedule_date);
            //}


            //ln->addLastOneTimeFLATSchedule_dateMonthly(first_repay_date_murad); 

        } else {
            schedule_date = ln->GetDisburse_date() + boost::gregorian::days(1);
            this->get_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());
            //std::cerr << "Disburse Date " << schedule_date << std::endl;
            samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
            samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;

            std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];
            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
            //std::cerr << "Grace Period " << ln->GetGrace_period() << std::endl;
            if (ln->GetGrace_period().compare("N_A") != 0) {
                schedule_date = schedule_date + boost::gregorian::days(1);
                schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
            }
            //std::cerr << "First Installment Date " << schedule_date << std::endl;
            int week_count = (*samity_schedules)[schedule_date];
            //std::cerr << "After Week Count " << week_count << std::endl;
            boost::gregorian::date last_date = ln->GetFirst_repayment_date();
            boost::gregorian::date pre_schedule_date = schedule_date;
            //last_date=ln->GetDisburse_date()+boost::gregorian::days(91*(ln->GetLoan_period_in_month()/3));
            ln->SetFirst_repayment_date(last_date);
            if ((*this->system_date) < ln->GetFirst_repayment_date()) {
                last_date = (*this->system_date); //+boost::gregorian::days(2);
            }
            //std::cerr<<"Samity_week_day "<<this->samity_list[samity_id]->GetSamity_week_day(schedule_date)<<" schedule_date "<<schedule_date<<"last_date "<<last_date<<" Disburse_date()"<<ln->GetDisburse_date()<<"\n";
            ////exit(1); 
            if (reschedule_list.empty())
                while (schedule_date <= last_date) {
                    pre_schedule_date = schedule_date;
                    //std::cerr<<" schedule_date "<<schedule_date<<" last_date "<<last_date<<std::endl;
                    ln->addNewOneTimeFLATSchedule_date(schedule_date);
                    count += 1;
                    week_count++;
                    //schedule_date = (*samity_schedules_index)[week_count];
                    this->get_weekly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());

                } else {
                while (schedule_date <= last_date) {
                    pre_schedule_date = schedule_date;
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
            }
            //if(schedule_date!=last_date){
            //ln->addNewOneTimeFLATSchedule_date(last_date);
            //}
            //last_date=ln->GetFirst_repayment_date(false);
            if (pre_schedule_date != last_date) {
                ln->addNewOneTimeFLATSchedule_date(last_date, false);
            }
            //if(pre_schedule_date==last_date){
            //last_date=last_date+boost::gregorian::days(1); 
            //}
            last_date = ln->GetFirst_repayment_date(false);
            //last_date=ln->GetDisburse_date()+boost::gregorian::days(90*(ln->GetLoan_period_in_month()/3));
            //std::cerr<<last_date<<"\n";//exit(1);
            ln->addLastOneTimeFLATSchedule_date(last_date);


        }
    } else if (ln->GetInterest_calculation_method().compare("F") == 0) {
                
        if (ln->GetRepayment_frequency().compare("D") == 0) {
            
            //std::cerr << "here " << schedule_date << std::endl;
            //exit(1);
            while (count < no_of_ins) {
                ln->addNewFLATSchedule_date(schedule_date);
                is_rescheduled = false;
                count += 1;
                this->get_daily_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());
                //this->get_weekly_valid_schedule_date(schedule_date, ln->GetSamity_id(),ln->GetId());
            }
                        
                boost::gregorian::date first_schedule_date = ln->GetFirst_repayment_date();
                if (ln->enforce_number_of_installment == 1 && ln->enforced_install_number>0) {
                    //std::cerr << "check Enforce " << ln->enforced_install_number << std::endl;
                    ln->addLastFLATSchedule_date_enforce(schedule_date, first_schedule_date, ln->enforced_install_number);                    
                } else {
                    ln->addLastFLATSchedule_date(schedule_date);
                }
            
            
            
            
        } else if (ln->GetRepayment_frequency().compare("W") == 0 && ln->service_charge_taken_time == "EFRAP") {
            samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
            samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;

            std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];
            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
            //std::cerr << "First Installment Date " << schedule_date << std::endl;
            int week_count = (*samity_schedules)[schedule_date];

            if (reschedule_list.empty())
                while (count < no_of_ins) {
                    ln->addNewEFRRAPSchedule_date(schedule_date);
                    //is_rescheduled = false;
                    count += 1;
                    week_count++;
                    schedule_date = (*samity_schedules_index)[week_count];

                } else
                while (count < no_of_ins) {

                    ln->addNewEFRRAPSchedule_date(schedule_date);

                    //is_rescheduled = false;
                    count += 1;
                    week_count++;
                    schedule_date = (*samity_schedules_index)[week_count];

                    for (it = reschedule_list.begin(); it != reschedule_list.end(); it++) {
                        if (schedule_date >= it->start && schedule_date <= it->end) {
                            schedule_date = it->end;
                            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                            week_count = (*samity_schedules)[schedule_date];
                            break;
                        }
                    }



                    //this->get_weekly_valid_schedule_date(schedule_date, ln->GetSamity_id(),ln->GetId());
                }
            ln->addLastEFRRAPSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("W") == 0 && ln->service_charge_taken_time == "SCTBI") {
            //schedule_date=schedule_date+boost::gregorian::weeks(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            //std::cerr << "I m here " << schedule_date << std::endl;
            samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
            samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;

            std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];
            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
            //std::cerr << "First Installment Date " << schedule_date << std::endl;
            int week_count = (*samity_schedules)[schedule_date];
            //std::cerr << "After Week Count " << week_count << std::endl;

            //this->samity_list[ln->GetSamity_id()]->print_samity_schedule();
            ////exit(1);
            if (reschedule_list.empty())
                while (count < no_of_ins) {
                    //std::cerr << "loop1  Week Count " << count << std::endl;    

                    ln->addNewEFRRAPSchedule_date_new(schedule_date);

                    //is_rescheduled = false;
                    count += 1;
                    week_count++;
                    schedule_date = (*samity_schedules_index)[week_count];

                } else
                while (count < no_of_ins) {

                    //std::cerr << "loop  Week Count " << count << std::endl;    

                    ln->addNewEFRRAPSchedule_date_new(schedule_date);

                    //is_rescheduled = false;
                    count += 1;
                    week_count++;
                    schedule_date = (*samity_schedules_index)[week_count];

                    for (it = reschedule_list.begin(); it != reschedule_list.end(); it++) {
                        if (schedule_date >= it->start && schedule_date <= it->end) {
                            schedule_date = it->end;
                            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                            week_count = (*samity_schedules)[schedule_date];
                            break;
                        }
                    }
                    //this->get_weekly_valid_schedule_date(schedule_date, ln->GetSamity_id(),ln->GetId());
                }
            ln->addLastEFRRAPSchedule_date_new(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("W") == 0) {
            //schedule_date=schedule_date+boost::gregorian::weeks(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            //std::cerr << "First Installment Date " << schedule_date << std::endl;
            samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
            samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;

            std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];
            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
            //std::cerr << "First Installment Date " << schedule_date << std::endl;
            int week_count = (*samity_schedules)[schedule_date];
            //std::cerr << "After Week Count " << week_count << std::endl;

            //this->samity_list[ln->GetSamity_id()]->print_samity_schedule();
            ////exit(1);
            if (reschedule_list.empty())
                while (count < no_of_ins) {
                    ln->addNewFLATSchedule_date(schedule_date);
                    //is_rescheduled = false;
                    count += 1;
                    week_count++;
                    schedule_date = (*samity_schedules_index)[week_count];
                } else
                while (count < no_of_ins) {



                    ln->addNewFLATSchedule_date(schedule_date);

                    //is_rescheduled = false;
                    count += 1;
                    week_count++;
                    schedule_date = (*samity_schedules_index)[week_count];

                    for (it = reschedule_list.begin(); it != reschedule_list.end(); it++) {
                        if (schedule_date >= it->start && schedule_date <= it->end) {
                            schedule_date = it->end;
                            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                            week_count = (*samity_schedules)[schedule_date];
                            break;
                        }
                    }



                    //this->get_weekly_valid_schedule_date(schedule_date, ln->GetSamity_id(),ln->GetId());
                }
            ln->addLastFLATSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("P") == 0) {
            //schedule_date=schedule_date+boost::gregorian::weeks(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            //std::cerr << ln->GetId() << " mmm " << schedule_date << std::endl;
            samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
            samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;
            std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];
            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
            //std::cerr << "First Installment Date " << schedule_date << std::endl;
            int week_count = (*samity_schedules)[schedule_date];
            //std::cerr << "After Week Count " << week_count << std::endl; 
            //this->samity_list[ln->GetSamity_id()]->print_samity_schedule();
            ////exit(1);
            if (reschedule_list.empty())
                while (count < no_of_ins) {
                    ln->addNewFLATSchedule_date(schedule_date);
                    //is_rescheduled = false;
                    count += 1;
                    week_count++;
                    schedule_date = (*samity_schedules_index)[week_count];

                } else
                while (count < no_of_ins) {
                    ln->addNewFLATSchedule_date(schedule_date);
                    //is_rescheduled = false;
                    count += 1;
                    week_count++;
                    schedule_date = (*samity_schedules_index)[week_count];

                    for (it = reschedule_list.begin(); it != reschedule_list.end(); it++) {
                        if (schedule_date >= it->start && schedule_date <= it->end) {
                            schedule_date = it->end;
                            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                            week_count = (*samity_schedules)[schedule_date];
                            break;
                        }
                    }
                    //this->get_weekly_valid_schedule_date(schedule_date, ln->GetSamity_id(),ln->GetId());
                }
            //std::cerr << ln->GetId() << " before " << schedule_date << std::endl;
            if (no_of_ins == 1) {
                schedule_date = ln->GetFirst_repayment_date();
                ln->addLastFLATSchedule_date(schedule_date);
            } else {
                ln->addLastFLATSchedule_date(schedule_date);
            }


        } else if (ln->GetRepayment_frequency().compare("F") == 0) {
            //schedule_date=schedule_date+boost::gregorian::weeks(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            //std::cerr << "First Installment Date " << schedule_date << std::endl;
            samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
            samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;

            std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];
            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
            //std::cerr << "First Installment Date " << schedule_date << std::endl;
            int week_count = (*samity_schedules)[schedule_date];
            //std::cerr << "After Week Count " << week_count << std::endl;

            //this->samity_list[ln->GetSamity_id()]->print_samity_schedule();
            ////exit(1);
            if (reschedule_list.empty())
                while (count < no_of_ins) {


                    ln->addNewFLATSchedule_date(schedule_date);

                    //is_rescheduled = false;
                    count += 1;
                    week_count += 2;
                    schedule_date = (*samity_schedules_index)[week_count];

                } else
                while (count < no_of_ins) {
                    ln->addNewFLATSchedule_date(schedule_date);
                    //is_rescheduled = false;
                    count += 1;
                    week_count += 2;
                    schedule_date = (*samity_schedules_index)[week_count];

                    for (it = reschedule_list.begin(); it != reschedule_list.end(); it++) {
                        if (schedule_date >= it->start && schedule_date <= it->end) {
                            schedule_date = it->end;
                            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                            week_count = (*samity_schedules)[schedule_date];
                            break;
                        }
                    }
                    //this->get_weekly_valid_schedule_date(schedule_date, ln->GetSamity_id(),ln->GetId());
                }
            ln->addLastFLATSchedule_date(schedule_date);
        }                 
        else if (ln->GetRepayment_frequency().compare("M") == 0 && ln->GetRepayment_day_or_week().compare("DD") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));

            boost::gregorian::date first_schedule_date = schedule_date;
            //std::cerr<<ln->repayment_mode_for_monthly_loan<< " GetRepayment_frequency:  " << ln->GetRepayment_frequency() << "GetRepayment_day_or_week: " << ln->GetRepayment_day_or_week()<< "service_charge_taken_time: " << ln->service_charge_taken_time<< std::endl;

            int repayment_week = this->get_loan_week_number_considering_date(first_schedule_date, ln->GetSamity_id(), ln->GetId());
            ////exit(1);
            //this->get_monthly_valid_schedule_date_consider_date(first_schedule_date, ln->GetSamity_id(), ln->GetId(), count,ln->GetRepayment_day(),ln->GetRepayment_week());
            if (ln->service_charge_taken_time == "EFS") {
                //std::cerr<<"In EFS\n";
                while (count < no_of_ins) {

                    ln->addNewMonthlyPricipleFirstSchedule_date(schedule_date);

                    is_rescheduled = false;

                    schedule_date = first_schedule_date;
                    //this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, 0, repayment_week);
                    if(ln->repayment_mode_for_monthly_loan == "SD" ||this->samity_list[ln->GetSamity_id()]->is_samity_day_obsolete==1){
                            this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                        }else{
                            this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, 0, repayment_week);
                    }
                    count += 1;
                }
                ln->addNewMonthlyPricipleLastSchedule_date(schedule_date);
            } else if (ln->service_charge_taken_time == "ASM") {
                //std::cerr<<"In ASM\n";
                while (count < no_of_ins) {

                    ln->addNewASMSchedule_date(schedule_date);

                    is_rescheduled = false;

                    schedule_date = first_schedule_date;
                    //this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                    if(ln->repayment_mode_for_monthly_loan == "SD" ||this->samity_list[ln->GetSamity_id()]->is_samity_day_obsolete==1){
                            this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                        }else{
                            this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, 0, repayment_week);
                     }
                    count += 1;
                }
                ln->addLastASMSchedule_date(schedule_date);
            } 
            else if (ln->service_charge_taken_time == "SCTBI") {
                //std::cerr<<"In ASM\n";
                    while (count < no_of_ins) {
                        
                        //ln->addNewEFRRAPSchedule_date_new(schedule_date);
                        ln->addNewSCTBISchedule_dateDFD(schedule_date);
                        
                        is_rescheduled = false;

                        schedule_date = first_schedule_date;                        
                        if(ln->repayment_mode_for_monthly_loan == "SD" ||this->samity_list[ln->GetSamity_id()]->is_samity_day_obsolete==1){
                            this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                        }else{
                            this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, 0, repayment_week);
                        }                        
                        count += 1;                   
                    }
                //ln->addLastEFRRAPSchedule_date_new(schedule_date);
                ln->addLastSCTBISchedule_dateDFD(schedule_date);
                
                 
            } 
            else {
                while (count < no_of_ins) {
           
					ln->addNewFLATSchedule_date(schedule_date);

                    is_rescheduled = false;

                    
                    if(ln->repayment_mode_for_monthly_loan == "SD" ||this->samity_list[ln->GetSamity_id()]->is_samity_day_obsolete==1){
                        schedule_date = first_schedule_date;
                        this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
//                        std::cerr << "111"<<std::endl;
                    }else{
                        this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, 0, repayment_week,false);
//                    std::cerr << "1112"<<std::endl;
                    }
                    count += 1;
                }
                ln->addLastFLATSchedule_date(schedule_date);
            }
        } else if (ln->GetRepayment_frequency().compare("M") == 0 && ln->GetRepayment_day_or_week().compare("NA") != 0) {
            boost::gregorian::date first_schedule_date = schedule_date;
           
            if (ln->service_charge_taken_time == "EFS") {
                //std::cerr<<"In EFS\n";
                while (count < no_of_ins) {

                    ln->addNewMonthlyPricipleFirstSchedule_date(schedule_date);

                    is_rescheduled = false;

                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                    count += 1;
                }
                ln->addNewMonthlyPricipleLastSchedule_date(schedule_date);
            } else if (ln->service_charge_taken_time == "ASM") {
                //std::cerr<<"In ASM\n";
                while (count < no_of_ins) {

                    ln->addNewASMSchedule_date(schedule_date);

                    is_rescheduled = false;

                    schedule_date = first_schedule_date;
                    //this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                    if(ln->repayment_mode_for_monthly_loan == "SD" ||this->samity_list[ln->GetSamity_id()]->is_samity_day_obsolete==1){
                            this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                    }else{
                            this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                     }
                    count += 1;
                }
                ln->addLastASMSchedule_date(schedule_date);
            } 
            else if (ln->service_charge_taken_time == "SCTBI") {
                
                    while (count < no_of_ins) {
                        
                        //ln->addNewEFRRAPSchedule_date_new(schedule_date);
                        ln->addNewSCTBISchedule_dateDFD(schedule_date);
                        
                        is_rescheduled = false;

                        schedule_date = first_schedule_date;                        
                        if(ln->repayment_mode_for_monthly_loan == "SD" ||this->samity_list[ln->GetSamity_id()]->is_samity_day_obsolete==1){
                            this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                        }else{
                            this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, 0, ln->GetRepayment_week());
                        }                        
                        count += 1;                   
                    }
                //ln->addLastEFRRAPSchedule_date_new(schedule_date);
                ln->addLastSCTBISchedule_dateDFD(schedule_date);
                
                 
            }
            else {
                while (count < no_of_ins) {

                    ln->addNewFLATSchedule_date(schedule_date);

                    is_rescheduled = false;

                    schedule_date = first_schedule_date;
                    if(ln->repayment_mode_for_monthly_loan == "SD" ||this->samity_list[ln->GetSamity_id()]->is_samity_day_obsolete==1){
                        this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                    }else{
                        this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                    }
                    count += 1;
                }
                ln->addLastFLATSchedule_date(schedule_date);
            }
        } else if (ln->GetRepayment_frequency().compare("M") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;
            //std::cerr<<"";
            while (count < no_of_ins) {

                ln->addNewFLATSchedule_date(schedule_date);

                is_rescheduled = false;

                schedule_date = first_schedule_date;
                if(ln->repayment_mode_for_monthly_loan == "SD" ||this->samity_list[samity_id]->is_samity_day_obsolete==1){
                    this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 0, ln->GetRepayment_day(), ln->GetRepayment_week());
                }else{
                    this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count);
                }
                count += 1;
            }
            ln->addLastFLATSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("2") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(2);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;

            while (count < no_of_ins) {



                ln->addNewFLATSchedule_date(schedule_date);

                is_rescheduled = false;


                schedule_date = first_schedule_date;
                this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 2 * count);
                count += 1;

            }
            ln->addLastFLATSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("3") == 0) {
            boost::gregorian::date first_schedule_date = schedule_date;
            if (ln->service_charge_taken_time == "ASM") {
                //std::cerr<<"In ASM\n";
                while (count < no_of_ins) {

                    ln->addNewASMSchedule_date(schedule_date);

                    is_rescheduled = false;

                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 3 * count, ln->GetRepayment_day(), ln->GetRepayment_week());
                    count += 1;
                }
                ln->addLastASMSchedule_date(schedule_date);
            } else {

                //schedule_date=schedule_date+boost::gregorian::months(3);
                //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
                //boost::gregorian::date first_schedule_date = schedule_date;
                while (count < no_of_ins) {



                    ln->addNewFLATSchedule_date(schedule_date);

                    is_rescheduled = false;

                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 3 * count);
                    count += 1;
                }
                ln->addLastFLATSchedule_date(schedule_date);
            }
        } else if (ln->GetRepayment_frequency().compare("4") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(4);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;
            while (count < no_of_ins) {



                ln->addNewFLATSchedule_date(schedule_date);

                is_rescheduled = false;

                schedule_date = first_schedule_date;
                this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 4 * count);
                count += 1;
            }
            ln->addLastFLATSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("5") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(5);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;
            while (count < no_of_ins) {
                ln->addNewFLATSchedule_date(schedule_date);

                is_rescheduled = false;

                schedule_date = first_schedule_date;
                this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 5 * count);
                count += 1;
            }
            ln->addLastFLATSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("6") == 0) {
            boost::gregorian::date first_schedule_date = schedule_date;
            if (ln->service_charge_taken_time == "ASM" && ln->GetRepayment_day_or_week().compare("DD") == 0) {
                while (count < no_of_ins) {
                    ln->addNewASMSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 6 * count);
                    count += 1;
                }
                ln->addLastASMSchedule_date(schedule_date);
            } else {
                boost::gregorian::date first_schedule_date = schedule_date;
                while (count < no_of_ins) {
                    ln->addNewFLATSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 6 * count);
                    count += 1;
                }
                ln->addLastFLATSchedule_date(schedule_date);
            }
            //schedule_date=schedule_date+boost::gregorian::months(6);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));

        } else if (ln->GetRepayment_frequency().compare("Y") == 0) {
            //boost::gregorian::date first_schedule_date = schedule_date;   
            if (ln->service_charge_taken_time == "ASM" && ln->GetRepayment_day_or_week().compare("DD") == 0) {
                //std::cerr<<"In ASM\n";
                while (count < no_of_ins) {
                    ln->addNewASMSchedule_date(schedule_date);
                    is_rescheduled = false;
                    //schedule_date = first_schedule_date;
                    this->get_yearly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());
                    count += 1;
                }
                ln->addLastASMSchedule_date(schedule_date);
            } else {
                //schedule_date=schedule_date+boost::gregorian::years(1);
                //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
                while (count < no_of_ins) {
                    ln->addNewFLATSchedule_date(schedule_date);
                    is_rescheduled = false;
                    count += 1;
                    this->get_yearly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());
                }
                ln->addLastFLATSchedule_date(schedule_date);
            }

        } else if (ln->GetRepayment_frequency().compare("O") == 0) {
            //schedule_date=schedule_date+boost::gregorian::years(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            ln->addNewOneTimeSchedule_date(schedule_date);
        }

    } else if (ln->GetInterest_calculation_method().compare("D") == 0) {
        no_of_ins = ln->actual_number_of_installments;
        if (ln->GetRepayment_frequency().compare("D") == 0) {
            //schedule_date=schedule_date+boost::gregorian::days(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            while (count < no_of_ins) {
                ln->addNewDECLININGSchedule_date(schedule_date);

                is_rescheduled = false;
                count += 1;

                this->get_daily_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());
            }
            ln->addLastDECLININGSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("W") == 0 && ln->service_charge_taken_time == "EFRAP") {
            //schedule_date=schedule_date+boost::gregorian::weeks(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
            samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;

            std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];

            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
            int week_count = (*samity_schedules)[schedule_date];
            if (reschedule_list.empty())
                while (count < no_of_ins) {


                    ln->addNewEFRRAPSchedule_date(schedule_date);

                    //is_rescheduled = false;
                    count += 1;
                    week_count++;
                    schedule_date = (*samity_schedules_index)[week_count];

                } else
                while (count < no_of_ins) {
                    ln->addNewEFRRAPSchedule_date(schedule_date);
                    //is_rescheduled = false;
                    count += 1;
                    week_count++;
                    schedule_date = (*samity_schedules_index)[week_count];

                    for (it = reschedule_list.begin(); it != reschedule_list.end(); it++) {
                        if (schedule_date >= it->start && schedule_date <= it->end) {
                            schedule_date = it->end;
                            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                            week_count = (*samity_schedules)[schedule_date];
                            break;
                        }
                    }
                }
            ln->addLastEFRRAPSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("W") == 0 && ln->GetDecline_Period() == "DB") {
            //schedule_date=schedule_date+boost::gregorian::weeks(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
            samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;

            std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];

            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
            int week_count = (*samity_schedules)[schedule_date];
            if (reschedule_list.empty())
                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date_daily_basis(schedule_date);
                    //is_rescheduled = false;
                    count += 1;
                    week_count++;
                    schedule_date = (*samity_schedules_index)[week_count];

                } else
                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date_daily_basis(schedule_date);
                    //is_rescheduled = false;
                    count += 1;
                    week_count++;
                    schedule_date = (*samity_schedules_index)[week_count];

                    for (it = reschedule_list.begin(); it != reschedule_list.end(); it++) {
                        if (schedule_date >= it->start && schedule_date <= it->end) {
                            schedule_date = it->end;
                            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                            week_count = (*samity_schedules)[schedule_date];
                            break;
                        }
                    }
                }
            ln->addLastDECLININGSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("W") == 0) {
            //schedule_date=schedule_date+boost::gregorian::weeks(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
            samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;

            std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];

            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
            int week_count = (*samity_schedules)[schedule_date];
            if (reschedule_list.empty())
                while (count < no_of_ins) {


                    ln->addNewDECLININGSchedule_date(schedule_date);

                    //is_rescheduled = false;
                    count += 1;
                    week_count++;
                    schedule_date = (*samity_schedules_index)[week_count];

                } else
                while (count < no_of_ins) {


                    ln->addNewDECLININGSchedule_date(schedule_date);

                    //is_rescheduled = false;
                    count += 1;
                    week_count++;
                    schedule_date = (*samity_schedules_index)[week_count];

                    for (it = reschedule_list.begin(); it != reschedule_list.end(); it++) {
                        if (schedule_date >= it->start && schedule_date <= it->end) {
                            schedule_date = it->end;
                            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                            week_count = (*samity_schedules)[schedule_date];
                            break;
                        }
                    }
                }
            ln->addLastDECLININGSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("F") == 0) {
            //schedule_date=schedule_date+boost::gregorian::weeks(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
            samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;

            std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];

            //std::cerr<<"Generating fortnightly declining schedule"<<std::endl;
            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
            int week_count = (*samity_schedules)[schedule_date];
            if (reschedule_list.empty()) {
                while (count < no_of_ins) {


                    ln->addNewDECLININGSchedule_date(schedule_date);

                    //is_rescheduled = false;
                    count += 1;
                    week_count += 2;
                    schedule_date = schedule_date + boost::gregorian::days(14);
                    //schedule_date = (*samity_schedules_index)[week_count];
                    schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                    //std::cerr<<" Initial "<<schedule_date<<std::endl;
                    while (1) {
                        if (schedule_date == (*samity_schedules_index)[week_count - 1]) {
                            week_count = (*samity_schedules)[schedule_date];
                            break;
                        } else if (schedule_date == (*samity_schedules_index)[week_count]) {
                            break;
                        } else {
                            schedule_date = schedule_date + boost::gregorian::days(7);
                            //std::cerr<<" prev "<<schedule_date<<std::endl;
                            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                            //std::cerr<<schedule_date<<std::endl;
                            week_count = (*samity_schedules)[schedule_date];
                        }
                    }
                    //std::cerr<<" Final "<<schedule_date<<std::endl;

                }
            } else {
                while (count < no_of_ins) {


                    ln->addNewDECLININGSchedule_date(schedule_date);

                    //is_rescheduled = false;
                    count += 1;
                    week_count += 2;

                    //schedule_date = (*samity_schedules_index)[week_count];
                    schedule_date = schedule_date + boost::gregorian::days(14);
                    //schedule_date = (*samity_schedules_index)[week_count];
                    schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                    while (1) {
                        if (schedule_date == (*samity_schedules_index)[week_count - 1]) {
                            week_count = (*samity_schedules)[schedule_date];
                            break;
                        } else if (schedule_date == (*samity_schedules_index)[week_count]) {
                            break;
                        } else {
                            schedule_date = schedule_date + boost::gregorian::days(7);
                            //std::cerr<<" prev "<<schedule_date<<std::endl;
                            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                            //std::cerr<<schedule_date<<std::endl;
                            week_count = (*samity_schedules)[schedule_date];
                        }
                    }
                    for (it = reschedule_list.begin(); it != reschedule_list.end(); it++) {
                        if (schedule_date >= it->start && schedule_date <= it->end) {
                            schedule_date = it->end;
                            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                            week_count = (*samity_schedules)[schedule_date];
                            break;
                        }
                    }
                    //std::cerr<<schedule_date<<std::endl;
                }
            }
            ln->addLastDECLININGSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("M") == 0 && ln->GetRepayment_day_or_week().compare("DD") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;
            int repayment_week = this->get_loan_week_number_considering_date(first_schedule_date, ln->GetSamity_id(), ln->GetId());
            //this->get_monthly_valid_schedule_date_consider_date(first_schedule_date, ln->GetSamity_id(), ln->GetId(), count,ln->GetRepayment_day(),ln->GetRepayment_week());
            if (ln->service_charge_taken_time == "EFS") {
                while (count < no_of_ins) {

                    ln->addNewMonthlyPricipleFirstSchedule_date(schedule_date);

                    is_rescheduled = false;

                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, 0, repayment_week);
                    count += 1;
                }
                ln->addNewMonthlyPricipleLastSchedule_date(schedule_date);
            } else if (ln->repayment_mode_for_monthly_loan == "SD") {
                first_schedule_date = ln->GetFirst_repayment_date();
                schedule_date = first_schedule_date;
                this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 0, ln->GetRepayment_day(), ln->GetRepayment_week());

                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                    count += 1;
                }

                ln->addLastDECLININGSchedule_date(schedule_date);

            } else if (ln->GetDecline_Period() == "DB") {
                first_schedule_date = ln->GetFirst_repayment_date();
                schedule_date = first_schedule_date;
                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date_daily_basis(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                    count += 1;
                }
                ln->addLastDECLININGSchedule_date(schedule_date);
            } else {
                while (count < no_of_ins) {

                    ln->addNewDECLININGSchedule_date(schedule_date);

                    is_rescheduled = false;

                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, 0, repayment_week);
                    count += 1;
                }
                ln->addLastDECLININGSchedule_date(schedule_date);
            }
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
            } else if (ln->repayment_mode_for_monthly_loan == "SD") {
                first_schedule_date = ln->GetFirst_repayment_date();
                schedule_date = first_schedule_date;
                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                    count += 1;
                }

                ln->addLastDECLININGSchedule_date(schedule_date);

            } else if (ln->GetDecline_Period() == "DB") {
                first_schedule_date = ln->GetFirst_repayment_date();
                schedule_date = first_schedule_date;
                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date_daily_basis(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                    count += 1;
                }
                ln->addLastDECLININGSchedule_date(schedule_date);
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
            //schedule_date=schedule_date+boost::gregorian::months(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            if (ln->GetDecline_Period() == "DB") {
                boost::gregorian::date first_schedule_date = schedule_date;
                first_schedule_date = ln->GetFirst_repayment_date();
                schedule_date = first_schedule_date;


                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date_daily_basis(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                    count += 1;
                }
                ln->addLastDECLININGSchedule_date(schedule_date);
            } else {

                boost::gregorian::date first_schedule_date = schedule_date;
                while (count < no_of_ins) {
                    ln->addNewDECLININGSchedule_date(schedule_date);

                    is_rescheduled = false;

                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count);
                    count += 1;
                }
                ln->addLastDECLININGSchedule_date(schedule_date);
            }

        } else if (ln->GetRepayment_frequency().compare("2") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(2);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;
            while (count < no_of_ins) {



                ln->addNewDECLININGSchedule_date(schedule_date);

                is_rescheduled = false;

                schedule_date = first_schedule_date;
                this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 2 * count);
                count += 1;
            }
            ln->addLastDECLININGSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("3") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(3);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;
            while (count < no_of_ins) {



                ln->addNewDECLININGSchedule_date(schedule_date);

                is_rescheduled = false;

                schedule_date = first_schedule_date;
                this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 3 * count);
                count += 1;
            }
            ln->addLastDECLININGSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("4") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(4);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;
            while (count < no_of_ins) {



                ln->addNewDECLININGSchedule_date(schedule_date);

                is_rescheduled = false;

                schedule_date = first_schedule_date;
                this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 4 * count);
                count += 1;
            }
            ln->addLastDECLININGSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("5") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(5);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;
            while (count < no_of_ins) {



                ln->addNewDECLININGSchedule_date(schedule_date);

                is_rescheduled = false;

                schedule_date = first_schedule_date;
                this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 5 * count);
                count += 1;
            }
            ln->addLastDECLININGSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("6") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(6);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;
            while (count < no_of_ins) {



                ln->addNewDECLININGSchedule_date(schedule_date);

                is_rescheduled = false;


                schedule_date = first_schedule_date;
                this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 6 * count);
                count += 1;
            }
            ln->addLastDECLININGSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("Y") == 0) {
            //schedule_date=schedule_date+boost::gregorian::years(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            while (count < no_of_ins) {



                ln->addNewDECLININGSchedule_date(schedule_date);

                is_rescheduled = false;
                count += 1;

                this->get_yearly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());
            }
            ln->addLastDECLININGSchedule_date(schedule_date);
        }
        //ln->addLastDECLININGSchedule_date(schedule_date);
    } else {
        // std::cerr << "New " << schedule_date << std::endl;

        no_of_ins = ln->actual_number_of_installments;
        if (ln->GetRepayment_frequency().compare("D") == 0) {
            //schedule_date=schedule_date+boost::gregorian::days(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            while (count < no_of_ins) {



                ln->addNewDFDSchedule_date(schedule_date);

                is_rescheduled = false;
                count += 1;

                this->get_daily_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());
            }
            ln->addLastDFDSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("W") == 0) {

            if (ln->service_charge_taken_time == "SCTBI") {
                //std::cerr<<" number_of_installment "<<std::endl;
                schedule_date = ln->GetDisburse_date() + boost::gregorian::days(1);
                //std::cerr<< "schedule date " <<schedule_date <<std::endl;
                this->get_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());
                samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
                samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;

                std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];
                schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                //std::cerr<< "schedule date " <<schedule_date <<std::endl;
                if (ln->GetGrace_period().compare("N_A") != 0) {
                    schedule_date = schedule_date + boost::gregorian::days(1);
                    schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                }
                int week_count = (*samity_schedules)[schedule_date];
                boost::gregorian::date last_date = ln->GetFirst_repayment_date();

                //std::cerr<< "Last date " << last_date <<std::endl;
                ln->SetFirst_repayment_date(last_date);
                schedule_date=schedule_date-boost::gregorian::weeks(1);
                this->get_weekly_valid_schedule_date_samity_day(schedule_date, ln->GetSamity_id(), ln->GetId());
                if (reschedule_list.empty()) {
                    while (schedule_date < last_date) {
                        //std::cerr<< "schedule date " <<schedule_date <<"last date "<< last_date <<std::endl;
                        ln->addNewSCTBISchedule_dateDFD(schedule_date);
                        count += 1;
                        week_count++;
                        this->get_weekly_valid_schedule_date_samity_day(schedule_date, ln->GetSamity_id(), ln->GetId());
                        //this->get_weekly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());
                    }
                    ln->addNewSCTBISchedule_dateDFD(last_date);
                } else {

                    while (schedule_date <= last_date) {
                        ln->addNewSCTBISchedule_dateDFD(schedule_date);
                        count += 1;
                        week_count++;
                        this->get_weekly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());

                        for (it = reschedule_list.begin(); it != reschedule_list.end(); it++) {
                            if (schedule_date >= it->start && schedule_date <= it->end) {
                                schedule_date = it->end;
                                this->get_weekly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());
                                break;
                            }
                        }
                    }
                }
            } else if(ln->repayment_mode_for_monthly_loan == "LISC"){
                samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
                samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;
                std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];
                schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                int week_count = (*samity_schedules)[schedule_date];
                double enforce_installment_amount=ln->GetTotal_payable_amount()-ln->installment_amnt*(ln->actual_number_of_installments-1);
                ln->interest_index=(ln->GetTotal_payable_amount()-enforce_installment_amount)/ln->GetLoan_amount();
                if (reschedule_list.empty()){
                    while (count < no_of_ins) {
                        ln->addNewDFDSchedule_date(schedule_date);
                        count += 1;
                        week_count++;
                        schedule_date = (*samity_schedules_index)[week_count];
                    } 
                }else{
                    while (count < no_of_ins) {
                        ln->addNewDFDSchedule_date(schedule_date);
                        //is_rescheduled = false;
                        count += 1;
                        week_count++;
                        schedule_date = (*samity_schedules_index)[week_count];
                        for (it = reschedule_list.begin(); it != reschedule_list.end(); it++) {
                            if (schedule_date >= it->start && schedule_date <= it->end) {
                                schedule_date = it->end;
                                schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                                week_count = (*samity_schedules)[schedule_date];
                                break;
                            }
                        }
                    }
                }
                //ln->addLastDFDSchedule_date(schedule_date);
                boost::gregorian::date first_schedule_date = ln->GetFirst_repayment_date();
                if (ln->enforce_number_of_installment == 1 && ln->enforced_install_number > 0) {
                    //std::cerr << "check Enforce " << ln->enforced_install_number << std::endl;
                    ln->addLastDFDSchedule_date_enforce(schedule_date, first_schedule_date, ln->enforced_install_number);
                } else {
                    ln->addLastDFDSchedule_date(schedule_date);
                }
            } else {

                //schedule_date=schedule_date+boost::gregorian::weeks(1);
                //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
                samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
                samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;

                std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];

                schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                int week_count = (*samity_schedules)[schedule_date];
                if (reschedule_list.empty())
                    while (count < no_of_ins) {

                        if (ln->service_charge_taken_time == "EFRAP") {
                            ln->addNewEFRRAPSchedule_date(schedule_date);
                        } else {
                            ln->addNewDFDSchedule_date(schedule_date);
                        }
                        //is_rescheduled = false;
                        count += 1;
                        week_count++;
                        schedule_date = (*samity_schedules_index)[week_count];

                    } else
                    while (count < no_of_ins) {


                        ln->addNewDFDSchedule_date(schedule_date);

                        //is_rescheduled = false;
                        count += 1;
                        week_count++;
                        schedule_date = (*samity_schedules_index)[week_count];

                        for (it = reschedule_list.begin(); it != reschedule_list.end(); it++) {
                            if (schedule_date >= it->start && schedule_date <= it->end) {
                                schedule_date = it->end;
                                schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                                week_count = (*samity_schedules)[schedule_date];
                                break;
                            }
                        }
                    }
                //ln->addLastDFDSchedule_date(schedule_date);
                boost::gregorian::date first_schedule_date = ln->GetFirst_repayment_date();
                if (ln->enforce_number_of_installment == 1 && ln->enforced_install_number > 0) {
                    //std::cerr << "check Enforce " << ln->enforced_install_number << std::endl;
                    ln->addLastDFDSchedule_date_enforce(schedule_date, first_schedule_date, ln->enforced_install_number);
                } else {
                    ln->addLastDFDSchedule_date(schedule_date);
                }
            }
        } else if (ln->GetRepayment_frequency().compare("F") == 0) {
            //schedule_date=schedule_date+boost::gregorian::weeks(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
            samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;

            std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];

            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
            //std::cerr<< "Schedule Date "<< schedule_date << std::endl;
            
            int week_count = (*samity_schedules)[schedule_date];
            if (reschedule_list.empty())
                while (count < no_of_ins) {
                    ln->addNewDFDSchedule_date(schedule_date);
                    //is_rescheduled = false;
                    count += 1;
                    week_count += 2;
                    schedule_date=schedule_date+ boost::gregorian::days(14);
                    schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                    
                    if (this->samity_holiday_list.find(samity_id) != this->samity_holiday_list.end()){
                        while (this->samity_holiday_list[samity_id].find(schedule_date) != this->samity_holiday_list[samity_id].end()) {
                            schedule_date = schedule_date + boost::gregorian::days(1);
                        }
                    }
                    while ((this->branch_holiday_list_p->find(schedule_date) != this->branch_holiday_list_p->end())) {
                        schedule_date = schedule_date + boost::gregorian::days(14);
                    }
                    
                } else
                while (count < no_of_ins) {
                    ln->addNewDFDSchedule_date(schedule_date);
                    //is_rescheduled = false;
                    count += 1;
                    week_count += 2;
                    schedule_date=schedule_date+ boost::gregorian::days(14);
                    //schedule_date = (*samity_schedules_index)[week_count];
                    
                    for (it = reschedule_list.begin(); it != reschedule_list.end(); it++) {
                        if (schedule_date >= it->start && schedule_date <= it->end) {
                            schedule_date = it->end;
                            schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                            week_count = (*samity_schedules)[schedule_date];
                            break;
                        }
                    }
                }
            ln->addLastDFDSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("P") == 0) {
            boost::gregorian::date first_schedule_date = schedule_date;
            if (ln->GetDecline_Period() == "DB") {
                // std::cerr << ln->GetId() << " before " << schedule_date << std::endl;
                if (ln->repayment_mode_for_monthly_loan == "SD") {
                    first_schedule_date = ln->GetFirst_repayment_date();
                    schedule_date = first_schedule_date;
                    while (count < no_of_ins) {
                        ln->addNewDFDOneTimeSchedule_dateDailyBasis(schedule_date);
                        is_rescheduled = false;
                        schedule_date = first_schedule_date;
                        this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                        count += 1;
                    }
                    ln->addNewDFDOneTimeSchedule_dateDailyBasis(schedule_date);

                } else {
                    ln->addNewDFDOneTimeSchedule_dateDailyBasis(schedule_date);
                }
            } else {

                samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
                samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;
                std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];
                schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                //std::cerr << "First Installment Date " << schedule_date << std::endl;
                int week_count = (*samity_schedules)[schedule_date];
                //std::cerr << "After Week Count " << week_count << std::endl; 
                //this->samity_list[ln->GetSamity_id()]->print_samity_schedule();
                ////exit(1);
                if (reschedule_list.empty())
                    while (count < no_of_ins) {
                        ln->addNewDFDSchedule_date(schedule_date);
                        //is_rescheduled = false;
                        count += 1;
                        week_count++;
                        schedule_date = (*samity_schedules_index)[week_count];

                    } else
                    while (count < no_of_ins) {
                        ln->addNewDFDSchedule_date(schedule_date);
                        //is_rescheduled = false;
                        count += 1;
                        week_count++;
                        schedule_date = (*samity_schedules_index)[week_count];

                        for (it = reschedule_list.begin(); it != reschedule_list.end(); it++) {
                            if (schedule_date >= it->start && schedule_date <= it->end) {
                                schedule_date = it->end;
                                schedule_date = boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id]->GetSamity_week_day(schedule_date));
                                week_count = (*samity_schedules)[schedule_date];
                                break;
                            }
                        }
                        //this->get_weekly_valid_schedule_date(schedule_date, ln->GetSamity_id(),ln->GetId());
                    }
                ln->addLastDFDSchedule_date(schedule_date);
            }

        } else if (ln->GetRepayment_frequency().compare("M") == 0 && ln->GetRepayment_day_or_week().compare("DD") != 0) {
            //std::cerr << "new monthly 3" << std::endl;
            // std::cerr << "Repayment_frequency " << ln->GetRepayment_frequency() << std::endl; 
            //  std::cerr << "Repayment_day_or_week " << ln->GetRepayment_day_or_week() << std::endl; 
            //schedule_date=schedule_date+boost::gregorian::months(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;
            int repayment_week = this->get_loan_week_number_considering_date(first_schedule_date, ln->GetSamity_id(), ln->GetId());
            //this->get_monthly_valid_schedule_date_consider_date(first_schedule_date, ln->GetSamity_id(), ln->GetId(), count,ln->GetRepayment_day(),ln->GetRepayment_week());
            if (ln->service_charge_taken_time == "EFS") {
                while (count < no_of_ins) {

                    ln->addNewMonthlyPricipleFirstSchedule_date(schedule_date);

                    is_rescheduled = false;

                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, 0, repayment_week);
                    count += 1;
                }
                ln->addNewMonthlyPricipleLastSchedule_date(schedule_date);
            } else if (ln->service_charge_taken_time == "SCTBI") {

                schedule_date = ln->GetDisburse_date() + boost::gregorian::months(1);
                this->get_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId());
                samity_schedules = this->samity_list[ln->GetSamity_id()]->samity_schedules;
                samity_schedules_index = this->samity_list[ln->GetSamity_id()]->samity_schedules_index;
                std::vector<date_pair> reschedule_list = loan_reschedule_list[ln->GetId()];
                boost::gregorian::date last_date = ln->GetFirst_repayment_date();
                int loan_period_in_month = ln->GetLoan_period_in_month();


                if (reschedule_list.empty()) {
                    while (schedule_date <= last_date) {
                        //std::cerr<<" MURAD empty S_DATE count " <<count<< " : " <<schedule_date<<" last_date "<<last_date<<std::endl;
                        ln->addNewSCTBISchedule_dateDFD(schedule_date);
                        is_rescheduled = false;
                        //schedule_date = (*samity_schedules_index)[week_count];
                        schedule_date = ln->GetDisburse_date() + boost::gregorian::months(1);
                        //this->get_weekly_valid_schedule_date(schedule_date,ln->GetSamity_id(),ln->GetId());
                        if (ln->GetDecline_Period() == "DB" && ln->GetNumber_of_installment() == 1) {
                            this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                        } else {
                            this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                        }
                        count += 1;
                        if (loan_period_in_month == count) {
                            break;
                        }
                        //week_count++; 
                    }
                }
                if (schedule_date > last_date) {
                    schedule_date = last_date;
                }
                ln->addLastSCTBISchedule_dateDFD(last_date);
            } else {
                while (count < no_of_ins) {

                    ln->addNewDFDSchedule_date(schedule_date);

                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                    count += 1;
                    //schedule_date = first_schedule_date;
                    // this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count,0,repayment_week);
                    //count += 1;
                }

                if (ln->enforce_number_of_installment == 1 && ln->enforced_install_number > 0) {
                    ln->addLastDFDSchedule_date_enforce(schedule_date, first_schedule_date, ln->enforced_install_number);
                } else {
                    ln->addLastDFDSchedule_date(schedule_date);
                }


            }
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
            } else if (ln->repayment_mode_for_monthly_loan == "SD") {
                first_schedule_date = ln->GetFirst_repayment_date();
                schedule_date = first_schedule_date;
                while (count < no_of_ins) {
                    ln->addNewDFDSchedule_date(schedule_date);
                    is_rescheduled = false;
                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_specific_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                    count += 1;
                }

                if (ln->enforce_number_of_installment == 1 && ln->enforced_install_number > 0) {
                    ln->addLastDFDSchedule_date_enforce(schedule_date, first_schedule_date, ln->enforced_install_number);
                } else {
                    ln->addLastDFDSchedule_date(schedule_date);
                }
            } else {
                while (count < no_of_ins) {

                    ln->addNewDFDSchedule_date(schedule_date);

                    is_rescheduled = false;

                    schedule_date = first_schedule_date;
                    this->get_monthly_valid_schedule_date_consider_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, ln->GetRepayment_day(), ln->GetRepayment_week());
                    count += 1;
                }
                if (ln->enforce_number_of_installment == 1 && ln->enforced_install_number > 0) {
                    ln->addLastDFDSchedule_date_enforce(schedule_date, first_schedule_date, ln->enforced_install_number);
                } else {
                    ln->addLastDFDSchedule_date(schedule_date);
                }
            }
        } else if (ln->GetRepayment_frequency().compare("M") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(1);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;
            while (count < no_of_ins) {



                ln->addNewDFDSchedule_date(schedule_date);

                is_rescheduled = false;

                schedule_date = first_schedule_date;
                this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count);
                count += 1;
            }
            ln->addLastDFDSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("G") == 0) {
            /******************As Per Grace Period [Start]************************************/
            boost::gregorian::date first_schedule_date = schedule_date;
            while (count < no_of_ins) {
                ln->addNewDFDSchedule_date(schedule_date);

                is_rescheduled = false;

                schedule_date = first_schedule_date;
                int grace_period = atoi(ln->GetGrace_period().c_str());
                this->get_grace_period_wise_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), count, grace_period);
                count += 1;
            }
            ln->addLastDFDSchedule_date(schedule_date);
            /******************As Per Grace Period [End]************************************/
        } else if (ln->GetRepayment_frequency().compare("3") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(3);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;
            while (count < no_of_ins) {
                ln->addNewDFDSchedule_date(schedule_date);

                is_rescheduled = false;

                schedule_date = first_schedule_date;
                this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 3 * count);
                count += 1;
            }
            ln->addLastDFDSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("4") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(4);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;
            while (count < no_of_ins) {
                ln->addNewDFDSchedule_date(schedule_date);

                is_rescheduled = false;

                schedule_date = first_schedule_date;
                this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 4 * count);
                count += 1;
            }
            ln->addLastDFDSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("5") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(5);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;
            while (count < no_of_ins) {
                ln->addNewDFDSchedule_date(schedule_date);

                is_rescheduled = false;

                schedule_date = first_schedule_date;
                this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 5 * count);
                count += 1;
            }
            ln->addLastDFDSchedule_date(schedule_date);
        } else if (ln->GetRepayment_frequency().compare("6") == 0) {
            //schedule_date=schedule_date+boost::gregorian::months(6);
            //schedule_date= boost::gregorian::next_weekday(schedule_date, this->samity_list[samity_id].GetSamity_week_day(schedule_date));
            boost::gregorian::date first_schedule_date = schedule_date;
            while (count < no_of_ins) {
                ln->addNewDFDSchedule_date(schedule_date);

                is_rescheduled = false;

                schedule_date = first_schedule_date;
                this->get_monthly_valid_schedule_date(schedule_date, ln->GetSamity_id(), ln->GetId(), 6 * count);
                count += 1;
            }
            ln->addLastDFDSchedule_date(schedule_date);
        }


    }
    ln->SetLast_repayment_date(schedule_date);

    ln->SetTotal_payable_amount();
    ln->SetInstallment_amount();

}

zval Loan_scheduler::get_overdue_loans_by_branch_samity(std::vector<int> samity_list, std::string s_date, int branch_id, int product_id) {

    boost::gregorian::date d_date(boost::gregorian::from_simple_string(s_date));

    //std::cerr << "\nOver Due Start" << d_date << "\n";
    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    typedef __gnu_cxx::hash_map<int, float> samity_recov;
    samity_recov samity_recovery;

    if (product_id) {
        int index = 0;
        for (int i = 0; i < samity_list.size(); i++) {

            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);
            int uu = 0;

            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;

            for (loan_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]][product_id].end(); it++) {
                samity_idd = samity_list[i];
                //zval *subarray;
                //MAKE_STD_ZVAL(subarray);
                //array_init(subarray);
                //if ((it->second->GetLoan_fully_paid_date() > d_date) && it->second->schedule_datum[it->second->GetActualNumber_of_installment() - 1].schedule_date < d_date) {
                if (it->second->GetLoan_fully_paid_date() > d_date) {
                    if (it->second->GetLast_repayment_date() < d_date) {
                        no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                        cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it->second->GetLoan_amount();
                        //add_assoc_long(subarray1, "loan_id", it->second->GetId());
                        add_index_long(&array1, index++, it->second->GetId());
                        //add_index_zval(array1,samity_list[i],subarray);
                    }
                }
                //add_index_zval(subarray3,samity_list[i],subarray);




            }

        }
    } else {
        int index = 0;
        for (int i = 0; i < samity_list.size(); i++) {

            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            int no_cumilative_expaired_borrower = 0;
            double cumilative_expaired_borrower_amount = 0;
            int samity_idd = 0;

            //std::cerr << "Samity ID"<< samity_list[i] << "\n";
            for (loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[i]].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[i]].end(); it++) {
                //std::cerr << " Branch ID"<<branch_id<<" Samity ID"<< samity_list[i] << "\n";
                samity_idd = samity_list[i];

                for (loan_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {

                    zval subarray;
//                    MAKE_STD_ZVAL(subarray);
                    array_init(&subarray);
                    //std::cerr <<" Loan ID "<<it1->second->GetId() <<" Number of installment "<< it1->second->GetActualNumber_of_installment()<< "System Date " << d_date <<" last schedule date " <<it1->second->schedule_datum[it1->second->GetActualNumber_of_installment() - 1].schedule_date <<"\n";
                    //std::cerr << " Branch ID "<<branch_id<<" Samity ID "<< samity_list[i]<< " Loan ID "<<it1->second->GetId() <<" Last Schedule date " << it1->second->GetLast_repayment_date() << "\n";
                    if (it1->second->GetLoan_fully_paid_date() > d_date) {
                        if (it1->second->GetLast_repayment_date() < d_date) {
                            no_cumilative_expaired_borrower = no_cumilative_expaired_borrower + 1;
                            cumilative_expaired_borrower_amount = cumilative_expaired_borrower_amount + it1->second->GetLoan_amount();
                            add_index_long(&array1, index++, it1->second->GetId());
                        }
                    }
                }
            }

        }

    }
    return array1;

}

zval Loan_scheduler::get_branch_wise_loan_information_before_date(int branch_id, std::string s_date, std::vector<int> samity_list, int product_id, int loan_id, bool include_fully_paid) {
    boost::gregorian::date d_date(boost::gregorian::from_simple_string(s_date));
    //std::cerr<< "get_branch_wise_loan_information_before_date\n";
    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    int samity_id;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_member_loan_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > member_loan_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;
    //branch_samity_loan_product_list[branch_id][samity_id][product_id]
    if (!samity_list.empty() && product_id && loan_id) {
        //std::cerr<< "samity_list product_id loan_id\n";

        typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
        typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
        typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

        //branch_samity_loan_product_list[branch_id][samity_id][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_id][product_id].end(); it++) {
        for (int samity_id_index = 0; samity_id_index < samity_list.size(); samity_id_index++) {
            //for (loan_hash::iterator it2 = branch_samity_member_loan_list[branch_id][samity_list[samity_id_index]][product_id].begin(); it2 != branch_samity_member_loan_list[branch_id][samity_list[samity_id_index]][product_id].end(); ++it2) {
            samity_id = samity_list[samity_id_index];
            loan* it2 = branch_samity_loan_product_list[branch_id][samity_list[samity_id_index]][product_id][loan_id];
            if (include_fully_paid == false && it2->GetLoan_fully_paid_date() < d_date)
                continue;
            double priciple_recoverable_before_date = 0;
            double total_recoverable_before_date = 0;
            double priciple_recoverable = 0;
            double total_recoverable = 0;
            std::string instalmanet_date = "";
            int installment_number = 0;
            zval subarray;
//            MAKE_STD_ZVAL(subarray);
            array_init(&subarray);

            for (int it3 = it2->getFirstInstallmentNumber(); it3 < it2->getLastInstallmentNumber(); it3++) {


                priciple_recoverable = priciple_recoverable + it2->schedule_datum[it3].principle_installment_amount;
                total_recoverable = total_recoverable + it2->schedule_datum[it3].installment_amount;

                if (it2->schedule_datum[it3].schedule_date <= d_date) {

                    priciple_recoverable_before_date = priciple_recoverable_before_date + it2->schedule_datum[it3].principle_installment_amount;
                    total_recoverable_before_date = total_recoverable_before_date + it2->schedule_datum[it3].installment_amount;

                    instalmanet_date.assign(boost::gregorian::to_iso_extended_string(it2->schedule_datum[it3].schedule_date).c_str());
                    installment_number = it3 + 1;
                }
            }
            if ((installment_number != it2->getFirstInstallmentNumber() || it2->GetDisburse_date() <= d_date) && samity_id == it2->GetSamity_id()) {
                add_assoc_long(&subarray, "loan_id", it2->GetId());
                add_assoc_long(&subarray, "member_id", it2->GetMember_id());
                add_assoc_long(&subarray, "branch_id", it2->GetBranch_id());
                add_assoc_long(&subarray, "samity_id", it2->GetSamity_id());
                add_assoc_long(&subarray, "product_id", it2->GetProduct_id());
                add_assoc_long(&subarray, "purpose_id", it2->GetPurpose_id());
                add_assoc_long(&subarray, "total_installment_number", it2->schedule_datum.size());
                add_assoc_double(&subarray, "priciple_recoverable", priciple_recoverable);
                add_assoc_double(&subarray, "total_recoverable", total_recoverable);
                add_assoc_double(&subarray, "priciple_recoverable_before_date", priciple_recoverable_before_date);
                add_assoc_double(&subarray, "total_recoverable_before_date", total_recoverable_before_date);

                add_assoc_string(&subarray, "disbursement_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->GetDisburse_date()).c_str()));
                add_assoc_string(&subarray, "first_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->schedule_datum[it2->getFirstInstallmentNumber()].schedule_date).c_str()));

                add_assoc_string(&subarray, "last_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->schedule_datum[it2->schedule_datum.size() - 1].schedule_date).c_str()));
                add_assoc_string(&subarray, "instalmanet_date", const_cast<char*> (instalmanet_date.c_str()));
                add_assoc_long(&subarray, "installment_number", installment_number);

                add_assoc_double(&subarray, "instalment_amount", it2->schedule_datum[(installment_number >= 0 ? installment_number : 0)].installment_amount);
                add_assoc_double(&subarray, "principle_instalment_amount", it2->schedule_datum[(installment_number >= 0 ? installment_number : 0)].principle_installment_amount);
                add_assoc_double(&subarray, "interest_instalment_amount", it2->schedule_datum[(installment_number >= 0 ? installment_number : 0)].interrest_installment_amount);

                add_assoc_double(&subarray, "last_instalment_amount", it2->schedule_datum[it2->schedule_datum.size() - 1].installment_amount);
                add_assoc_double(&subarray, "last_principle_instalment_amount", it2->schedule_datum[it2->schedule_datum.size() - 1].principle_installment_amount);
                add_assoc_double(&subarray, "last_interest_instalment_amount", it2->schedule_datum[it2->schedule_datum.size() - 1].interrest_installment_amount);

                add_assoc_long(&subarray, "number_of_installment", it2->GetNumber_of_installment());
                add_assoc_string(&subarray, "repayment_frequency", const_cast<char*> (it2->GetRepayment_frequency().c_str()));
                add_assoc_double(&subarray, "loan_period_in_month", it2->GetLoan_period_in_month());

                add_assoc_double(&subarray, "installment_amount", it2->schedule_datum[0].installment_amount);
                add_assoc_string(&subarray, "loan_type", const_cast<char*> (it2->GetLoan_type().c_str()));

                add_assoc_double(&subarray, "todays_installment", 0);
                for (int j = it2->getFirstInstallmentNumber(); j < it2->getLastInstallmentNumber(); j++) {
                    if (it2->schedule_datum[j].schedule_date == d_date) {
                        add_assoc_double(&subarray, "todays_installment", it2->schedule_datum[j].installment_amount);
                        break;
                    }
                }
                add_assoc_string(&subarray, "transfer_in_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->GetTransfer_in_date()).c_str()));
                add_assoc_string(&subarray, "transfer_out_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->GetTransfer_out_date()).c_str()));
                add_index_zval(&array1, it2->GetId(), &subarray);
            }
        }

    } else if (!samity_list.empty() && product_id) {
        //std::cerr<< "samity_list product_id\n";

        typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
        typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
        typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

        //branch_samity_loan_product_list[branch_id][samity_id][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_id][product_id].end(); it++) {
        for (int samity_id_index = 0; samity_id_index < samity_list.size(); samity_id_index++) {
            samity_id = samity_list[samity_id_index];

            //std::cerr<<samity_list[samity_id_index]<<std::endl;

            for (loan_hash::iterator it2 = branch_samity_loan_product_list[branch_id][samity_list[samity_id_index]][product_id].begin(); it2 != branch_samity_loan_product_list[branch_id][samity_list[samity_id_index]][product_id].end(); ++it2) {
                //std::cerr<<it2->second->GetId()<<std::endl;
                if (include_fully_paid == false && it2->second->GetLoan_fully_paid_date() < d_date)
                    continue;
                double priciple_recoverable_before_date = 0;
                double total_recoverable_before_date = 0;
                double priciple_recoverable = 0;
                double total_recoverable = 0;
                std::string instalmanet_date = "";
                int installment_number = 0;
                zval subarray;
//                MAKE_STD_ZVAL(subarray);
                array_init(&subarray);

                for (int it3 = it2->second->getFirstInstallmentNumber(); it3 < it2->second->getLastInstallmentNumber(); it3++) {
                    //for (int it3 = 0; it3 < it2->second->getLastInstallmentNumber(); it3++) {


                    priciple_recoverable = priciple_recoverable + it2->second->schedule_datum[it3].principle_installment_amount;
                    total_recoverable = total_recoverable + it2->second->schedule_datum[it3].installment_amount;

                    if (it2->second->schedule_datum[it3].schedule_date <= d_date) {

                        priciple_recoverable_before_date = priciple_recoverable_before_date + it2->second->schedule_datum[it3].principle_installment_amount;
                        total_recoverable_before_date = total_recoverable_before_date + it2->second->schedule_datum[it3].installment_amount;

                        instalmanet_date.assign(boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it3].schedule_date).c_str());
                        installment_number = it3 + 1;
                    }
                }
                if ((installment_number != it2->second->getFirstInstallmentNumber() || it2->second->GetDisburse_date() <= d_date) && samity_id == it2->second->GetSamity_id()) {
                    add_assoc_long(&subarray, "loan_id", it2->second->GetId());
                    add_assoc_long(&subarray, "member_id", it2->second->GetMember_id());
                    add_assoc_long(&subarray, "branch_id", it2->second->GetBranch_id());
                    add_assoc_long(&subarray, "samity_id", it2->second->GetSamity_id());
                    add_assoc_long(&subarray, "product_id", it2->second->GetProduct_id());
                    add_assoc_long(&subarray, "purpose_id", it2->second->GetPurpose_id());
                    add_assoc_long(&subarray, "total_installment_number", it2->second->schedule_datum.size());
                    add_assoc_double(&subarray, "priciple_recoverable", priciple_recoverable);
                    add_assoc_double(&subarray, "total_recoverable", total_recoverable);
                    add_assoc_double(&subarray, "priciple_recoverable_before_date", priciple_recoverable_before_date);
                    add_assoc_double(&subarray, "total_recoverable_before_date", total_recoverable_before_date);

                    add_assoc_string(&subarray, "disbursement_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->GetDisburse_date()).c_str()));
                    add_assoc_string(&subarray, "first_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it2->second->getFirstInstallmentNumber()].schedule_date).c_str()));

                    add_assoc_string(&subarray, "last_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].schedule_date).c_str()));
                    add_assoc_string(&subarray, "instalmanet_date", const_cast<char*> (instalmanet_date.c_str()));
                    add_assoc_long(&subarray, "installment_number", installment_number);

                    add_assoc_double(&subarray, "instalment_amount", it2->second->schedule_datum[(installment_number >= 0 ? installment_number : 0)].installment_amount);
                    add_assoc_double(&subarray, "principle_instalment_amount", it2->second->schedule_datum[(installment_number >= 0 ? installment_number : 0)].principle_installment_amount);
                    add_assoc_double(&subarray, "interest_instalment_amount", it2->second->schedule_datum[(installment_number >= 0 ? installment_number : 0)].interrest_installment_amount);

                    add_assoc_double(&subarray, "last_instalment_amount", it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].installment_amount);
                    add_assoc_double(&subarray, "last_principle_instalment_amount", it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].principle_installment_amount);
                    add_assoc_double(&subarray, "last_interest_instalment_amount", it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].interrest_installment_amount);

                    add_assoc_long(&subarray, "number_of_installment", it2->second->GetNumber_of_installment());
                    add_assoc_string(&subarray, "repayment_frequency", const_cast<char*> (it2->second->GetRepayment_frequency().c_str()));
                    add_assoc_double(&subarray, "loan_period_in_month", it2->second->GetLoan_period_in_month());

                    add_assoc_double(&subarray, "installment_amount", it2->second->schedule_datum[0].installment_amount);
                    add_assoc_string(&subarray, "loan_type", const_cast<char*> (it2->second->GetLoan_type().c_str()));

                    add_assoc_double(&subarray, "todays_installment", 0);
                    for (int j = it2->second->getFirstInstallmentNumber(); j < it2->second->getLastInstallmentNumber(); j++) {
                        if (it2->second->schedule_datum[j].schedule_date == d_date) {
                            add_assoc_double(&subarray, "todays_installment", it2->second->schedule_datum[j].installment_amount);
                            break;
                        }
                    }
                    add_assoc_string(&subarray, "transfer_in_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->GetTransfer_in_date()).c_str()));
                    add_assoc_string(&subarray, "transfer_out_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->GetTransfer_out_date()).c_str()));
                    add_index_zval(&array1, it2->second->GetId(), &subarray);
                }
            }
        }

    } else if (!samity_list.empty()) {
        //std::cerr<< "samity_list\n";
        typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_loan_product_hash;
        typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > loan_product_hash;
        typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

        //branch_samity_loan_product_list[branch_id][samity_id][product_id].begin(); it != branch_samity_loan_product_list[branch_id][samity_id][product_id].end(); it++) {
        for (int samity_id_index = 0; samity_id_index < samity_list.size(); samity_id_index++) {
            samity_id = samity_list[samity_id_index];

            for (loan_product_hash::iterator it = branch_samity_loan_product_list[branch_id][samity_list[samity_id_index]].begin(); it != branch_samity_loan_product_list[branch_id][samity_list[samity_id_index]].end(); ++it) {
                for (loan_hash::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                    if (include_fully_paid == false && it2->second->GetLoan_fully_paid_date() < d_date)
                        continue;

                    double priciple_recoverable_before_date = 0;
                    double total_recoverable_before_date = 0;
                    double priciple_recoverable = 0;
                    double total_recoverable = 0;
                    std::string instalmanet_date = "";
                    int installment_number = 0;
                    zval subarray;
//                    MAKE_STD_ZVAL(subarray);
                    array_init(&subarray);

                    //for (int it3 = 0; it3 < it2->second->getLastInstallmentNumber(); it3++) {
                    for (int it3 = it2->second->getFirstInstallmentNumber(); it3 < it2->second->getLastInstallmentNumber(); it3++) {


                        priciple_recoverable = priciple_recoverable + it2->second->schedule_datum[it3].principle_installment_amount;
                        total_recoverable = total_recoverable + it2->second->schedule_datum[it3].installment_amount;

                        if (it2->second->schedule_datum[it3].schedule_date <= d_date) {

                            priciple_recoverable_before_date = priciple_recoverable_before_date + it2->second->schedule_datum[it3].principle_installment_amount;
                            total_recoverable_before_date = total_recoverable_before_date + it2->second->schedule_datum[it3].installment_amount;

                            instalmanet_date.assign(boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it3].schedule_date).c_str());
                            installment_number = it3 + 1;
                        }
                    }
                    if ((installment_number != it2->second->getFirstInstallmentNumber() || it2->second->GetDisburse_date() <= d_date) && samity_id == it2->second->GetSamity_id()) {
                        add_assoc_long(&subarray, "loan_id", it2->second->GetId());
                        add_assoc_long(&subarray, "member_id", it2->second->GetMember_id());
                        add_assoc_long(&subarray, "branch_id", it2->second->GetBranch_id());
                        add_assoc_long(&subarray, "samity_id", it2->second->GetSamity_id());
                        add_assoc_long(&subarray, "product_id", it2->second->GetProduct_id());
                        add_assoc_long(&subarray, "purpose_id", it2->second->GetPurpose_id());
                        add_assoc_long(&subarray, "total_installment_number", it2->second->schedule_datum.size());
                        add_assoc_double(&subarray, "priciple_recoverable", priciple_recoverable);
                        add_assoc_double(&subarray, "total_recoverable", total_recoverable);
                        add_assoc_double(&subarray, "priciple_recoverable_before_date", priciple_recoverable_before_date);
                        add_assoc_double(&subarray, "total_recoverable_before_date", total_recoverable_before_date);

                        add_assoc_string(&subarray, "disbursement_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->GetDisburse_date()).c_str()));
                        add_assoc_string(&subarray, "first_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it2->second->getFirstInstallmentNumber()].schedule_date).c_str()));

                        add_assoc_string(&subarray, "last_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].schedule_date).c_str()));
                        add_assoc_string(&subarray, "instalmanet_date", const_cast<char*> (instalmanet_date.c_str()));
                        add_assoc_long(&subarray, "installment_number", installment_number);

                        add_assoc_double(&subarray, "instalment_amount", it2->second->schedule_datum[(installment_number >= 0 ? installment_number : 0)].installment_amount);
                        add_assoc_double(&subarray, "principle_instalment_amount", it2->second->schedule_datum[(installment_number >= 0 ? installment_number : 0)].principle_installment_amount);
                        add_assoc_double(&subarray, "interest_instalment_amount", it2->second->schedule_datum[(installment_number >= 0 ? installment_number : 0)].interrest_installment_amount);

                        add_assoc_double(&subarray, "last_instalment_amount", it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].installment_amount);
                        add_assoc_double(&subarray, "last_principle_instalment_amount", it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].principle_installment_amount);
                        add_assoc_double(&subarray, "last_interest_instalment_amount", it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].interrest_installment_amount);

                        add_assoc_long(&subarray, "number_of_installment", it2->second->GetNumber_of_installment());
                        add_assoc_string(&subarray, "repayment_frequency", const_cast<char*> (it2->second->GetRepayment_frequency().c_str()));
                        add_assoc_double(&subarray, "loan_period_in_month", it2->second->GetLoan_period_in_month());

                        add_assoc_double(&subarray, "installment_amount", it2->second->schedule_datum[0].installment_amount);
                        add_assoc_string(&subarray, "loan_type", const_cast<char*> (it2->second->GetLoan_type().c_str()));

                        add_assoc_double(&subarray, "todays_installment", 0);
                        for (int j = it2->second->getFirstInstallmentNumber(); j < it2->second->getLastInstallmentNumber(); j++) {
                            if (it2->second->schedule_datum[j].schedule_date == d_date) {
                                add_assoc_double(&subarray, "todays_installment", it2->second->schedule_datum[j].installment_amount);
                                break;
                            }
                        }
                        add_assoc_string(&subarray, "transfer_in_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->GetTransfer_in_date()).c_str()));
                        add_assoc_string(&subarray, "transfer_out_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->GetTransfer_out_date()).c_str()));
                        add_index_zval(&array1, it2->second->GetId(), &subarray);
                    }

                    //add_next_index_zval(array1, subarray);
                }
            }
        }

    } else {


        for (samity_member_loan_hash::iterator it = branch_samity_member_loan_list[branch_id].begin(); it != branch_samity_member_loan_list[branch_id].end(); ++it) {

            for (member_loan_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); ++it1) {

                for (loan_hash::iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
                    if (include_fully_paid == false && it2->second->GetLoan_fully_paid_date() < d_date)
                        continue;
                    double priciple_recoverable_before_date = 0;
                    double total_recoverable_before_date = 0;
                    double priciple_recoverable = 0;
                    double total_recoverable = 0;
                    std::string instalmanet_date = "";
                    int installment_number = 0;
                    zval subarray;
//                    MAKE_STD_ZVAL(subarray);
                    array_init(&subarray);

                    for (int it3 = it2->second->getFirstInstallmentNumber(); it3 < it2->second->getLastInstallmentNumber(); it3++) {
                        //for (int it3 = 0; it3 < it2->second->getLastInstallmentNumber(); it3++) {


                        priciple_recoverable = priciple_recoverable + it2->second->schedule_datum[it3].principle_installment_amount;
                        total_recoverable = total_recoverable + it2->second->schedule_datum[it3].installment_amount;

                        if (it2->second->schedule_datum[it3].schedule_date <= d_date) {

                            priciple_recoverable_before_date = priciple_recoverable_before_date + it2->second->schedule_datum[it3].principle_installment_amount;
                            total_recoverable_before_date = total_recoverable_before_date + it2->second->schedule_datum[it3].installment_amount;

                            instalmanet_date.assign(boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it3].schedule_date).c_str());
                            installment_number = it3 + 1;
                        }
                    }
                    if (installment_number != it2->second->getFirstInstallmentNumber() || it2->second->GetDisburse_date() <= d_date) {
                        add_assoc_long(&subarray, "loan_id", it2->second->GetId());
                        add_assoc_long(&subarray, "member_id", it2->second->GetMember_id());
                        add_assoc_long(&subarray, "branch_id", it2->second->GetBranch_id());
                        add_assoc_long(&subarray, "samity_id", it2->second->GetSamity_id());
                        add_assoc_long(&subarray, "product_id", it2->second->GetProduct_id());
                        add_assoc_long(&subarray, "purpose_id", it2->second->GetPurpose_id());
                        add_assoc_long(&subarray, "total_installment_number", it2->second->schedule_datum.size());
                        add_assoc_double(&subarray, "priciple_recoverable", priciple_recoverable);
                        add_assoc_double(&subarray, "total_recoverable", total_recoverable);
                        add_assoc_double(&subarray, "priciple_recoverable_before_date", priciple_recoverable_before_date);
                        add_assoc_double(&subarray, "total_recoverable_before_date", total_recoverable_before_date);

                        add_assoc_string(&subarray, "disbursement_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->GetDisburse_date()).c_str()));
                        add_assoc_string(&subarray, "first_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it2->second->getFirstInstallmentNumber()].schedule_date).c_str()));

                        add_assoc_string(&subarray, "last_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].schedule_date).c_str()));
                        add_assoc_string(&subarray, "instalmanet_date", const_cast<char*> (instalmanet_date.c_str()));
                        add_assoc_long(&subarray, "installment_number", installment_number);

                        add_assoc_double(&subarray, "instalment_amount", it2->second->schedule_datum[(installment_number >= 0 ? installment_number : 0)].installment_amount);
                        add_assoc_double(&subarray, "principle_instalment_amount", it2->second->schedule_datum[(installment_number >= 0 ? installment_number : 0)].principle_installment_amount);
                        add_assoc_double(&subarray, "interest_instalment_amount", it2->second->schedule_datum[(installment_number >= 0 ? installment_number : 0)].interrest_installment_amount);

                        add_assoc_double(&subarray, "last_instalment_amount", it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].installment_amount);
                        add_assoc_double(&subarray, "last_principle_instalment_amount", it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].principle_installment_amount);
                        add_assoc_double(&subarray, "last_interest_instalment_amount", it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].interrest_installment_amount);

                        add_assoc_long(&subarray, "number_of_installment", it2->second->GetNumber_of_installment());
                        add_assoc_string(&subarray, "repayment_frequency", const_cast<char*> (it2->second->GetRepayment_frequency().c_str()));
                        add_assoc_double(&subarray, "loan_period_in_month", it2->second->GetLoan_period_in_month());

                        add_assoc_double(&subarray, "installment_amount", it2->second->schedule_datum[0].installment_amount);
                        add_assoc_string(&subarray, "loan_type", const_cast<char*> (it2->second->GetLoan_type().c_str()));

                        add_assoc_double(&subarray, "todays_installment", 0);
                        for (int j = it2->second->getFirstInstallmentNumber(); j < it2->second->getLastInstallmentNumber(); j++) {
                            if (it2->second->schedule_datum[j].schedule_date == d_date) {
                                add_assoc_double(&subarray, "todays_installment", it2->second->schedule_datum[j].installment_amount);
                                break;
                            }
                        }
                        add_assoc_string(&subarray, "transfer_in_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->GetTransfer_in_date()).c_str()));
                        add_assoc_string(&subarray, "transfer_out_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->GetTransfer_out_date()).c_str()));
                        add_index_zval(&array1, it2->second->GetId(), &subarray);
                    }

                    //add_next_index_zval(array1, subarray);
                }
            }
        }
    }

    //std::cerr << "Before return" << std::endl;
    return array1;



}

zval Loan_scheduler::get_branch_wise_loan_information_between_date(int branch_id, std::string s_date_frm, std::string s_date_to) {

    //std::cerr<<branch_id<<" "<<s_date_frm<<" "<<s_date_to<<std::endl;
    //exit(1);
    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    //std::cerr<<"In get_branch_wise_loan_information_between_date"<<" "<<branch_id<<" "<<s_date_frm<<" "<<s_date_to<<std::endl;;

    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > > samity_member_loan_hash;
    typedef __gnu_cxx::hash_map<int, __gnu_cxx::hash_map<int, loan*> > member_loan_hash;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;

    if (s_date_to.empty()) {
        boost::gregorian::date d_date(boost::gregorian::from_simple_string(s_date_frm));
        for (samity_member_loan_hash::iterator it = branch_samity_member_loan_list[branch_id].begin(); it != branch_samity_member_loan_list[branch_id].end(); ++it) {

            for (member_loan_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); ++it1) {

                for (loan_hash::iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {

                    double priciple_recoverable_before_date = 0;
                    double total_recoverable_before_date = 0;
                    double priciple_recoverable = 0;
                    double total_recoverable = 0;
                    std::string instalmanet_date = "";
                    int installment_number = 0;
                    zval subarray;
//                    MAKE_STD_ZVAL(subarray);
                    array_init(&subarray);

                    for (int it3 = it2->second->getFirstInstallmentNumber(); it3 < it2->second->getLastInstallmentNumber(); it3++) {


                        priciple_recoverable = priciple_recoverable + it2->second->schedule_datum[it3].principle_installment_amount;
                        total_recoverable = total_recoverable + it2->second->schedule_datum[it3].installment_amount;


                        if (it2->second->schedule_datum[it3].schedule_date <= d_date) {

                            priciple_recoverable_before_date = priciple_recoverable_before_date + it2->second->schedule_datum[it3].principle_installment_amount;
                            total_recoverable_before_date = total_recoverable_before_date + it2->second->schedule_datum[it3].installment_amount;

                            //std::cerr<<"Without Date To priciple_recoverable "<<priciple_recoverable<<" total_recoverable "<<total_recoverable<<std::endl;

                            instalmanet_date.assign(boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it3].schedule_date).c_str());
                            installment_number = it3 + 1;

                            //std::cerr<<"instalmanet_date "<<instalmanet_date<<" installment_number "<<installment_number<<std::endl;

                        }
                    }
                    if (installment_number != it2->second->getFirstInstallmentNumber()) {
                        add_assoc_long(&subarray, "loan_id", it2->second->GetId());
                        add_assoc_long(&subarray, "member_id", it2->second->GetMember_id());
                        add_assoc_long(&subarray, "branch_id", it2->second->GetBranch_id());
                        add_assoc_long(&subarray, "samity_id", it2->second->GetSamity_id());
                        add_assoc_long(&subarray, "product_id", it2->second->GetProduct_id());
                        add_assoc_long(&subarray, "total_installment_number", it2->second->schedule_datum.size());
                        add_assoc_double(&subarray, "priciple_recoverable", priciple_recoverable);
                        add_assoc_double(&subarray, "total_recoverable", total_recoverable);
                        add_assoc_double(&subarray, "priciple_recoverable_between_date", priciple_recoverable_before_date);
                        add_assoc_double(&subarray, "total_recoverable_between_date", total_recoverable_before_date);

                        add_assoc_string(&subarray, "disbursement_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->GetDisburse_date()).c_str()));
                        add_assoc_string(&subarray, "first_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it2->second->getFirstInstallmentNumber()].schedule_date).c_str()));

                        add_assoc_string(&subarray, "last_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].schedule_date).c_str()));
                        add_assoc_string(&subarray, "instalmanet_date", const_cast<char*> (instalmanet_date.c_str()));
                        add_assoc_long(&subarray, "installment_number", installment_number);
                        add_assoc_string(&subarray, "loan_type", const_cast<char*>(it2->second->GetLoan_type().c_str()));
                        add_index_zval(&array1, it2->second->GetId(), &subarray);
                    } else if (it2->second->GetDisburse_date() <= d_date) {
                        add_assoc_long(&subarray, "loan_id", it2->second->GetId());
                        add_assoc_long(&subarray, "member_id", it2->second->GetMember_id());
                        add_assoc_long(&subarray, "branch_id", it2->second->GetBranch_id());
                        add_assoc_long(&subarray, "samity_id", it2->second->GetSamity_id());
                        add_assoc_long(&subarray, "product_id", it2->second->GetProduct_id());
                        add_assoc_long(&subarray, "total_installment_number", it2->second->schedule_datum.size());
                        add_assoc_double(&subarray, "priciple_recoverable", priciple_recoverable);
                        add_assoc_double(&subarray, "total_recoverable", total_recoverable);
                        add_assoc_double(&subarray, "priciple_recoverable_between_date", priciple_recoverable_before_date);
                        add_assoc_double(&subarray, "total_recoverable_between_date", total_recoverable_before_date);

                        add_assoc_string(&subarray, "disbursement_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->GetDisburse_date()).c_str()));
                        add_assoc_string(&subarray, "first_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it2->second->getFirstInstallmentNumber()].schedule_date).c_str()));

                        add_assoc_string(&subarray, "last_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].schedule_date).c_str()));
                        add_assoc_string(&subarray, "instalmanet_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->GetDisburse_date()).c_str()));
                        add_assoc_long(&subarray, "installment_number", 0);
                        add_assoc_string(&subarray, "loan_type", const_cast<char*>(it2->second->GetLoan_type().c_str()));
                        add_index_zval(&array1, it2->second->GetId(), &subarray);
                    }

                    //add_next_index_zval(array1, subarray);
                }
            }
        }
    } else {
        boost::gregorian::date d_date_frm(boost::gregorian::from_simple_string(s_date_frm));
        boost::gregorian::date d_date_to(boost::gregorian::from_simple_string(s_date_to));
        for (samity_member_loan_hash::iterator it = branch_samity_member_loan_list[branch_id].begin(); it != branch_samity_member_loan_list[branch_id].end(); ++it) {

            for (member_loan_hash::iterator it1 = it->second.begin(); it1 != it->second.end(); ++it1) {
                //std::cerr<<"Member Index"<<it1->first<<std::endl;
                for (loan_hash::iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
                    //std::cerr<<"Loan Index"<<it2->first<<std::endl;

                    double priciple_recoverable_before_date = 0;
                    double total_recoverable_before_date = 0;
                    double priciple_recoverable_between_date = 0;
                    double total_recoverable_between_date = 0;
                    double priciple_recoverable = 0;
                    double total_recoverable = 0;
                    std::string instalmanet_date = "";
                    int installment_number = -1;
                    zval subarray;
//                    MAKE_STD_ZVAL(subarray);
                    array_init(&subarray);

                    int less_than_flag = -1;

                    for (int it3 = it2->second->getFirstInstallmentNumber(); it3 < it2->second->getLastInstallmentNumber(); it3++) {


                        priciple_recoverable = priciple_recoverable + it2->second->schedule_datum[it3].principle_installment_amount;
                        total_recoverable = total_recoverable + it2->second->schedule_datum[it3].installment_amount;

                        //std::cerr<<"with date to priciple_recoverable "<<priciple_recoverable<<" total_recoverable "<<total_recoverable;



                        if (it2->second->schedule_datum[it3].schedule_date <= d_date_frm) {

                            priciple_recoverable_before_date = priciple_recoverable_before_date + it2->second->schedule_datum[it3].principle_installment_amount;
                            total_recoverable_before_date = total_recoverable_before_date + it2->second->schedule_datum[it3].installment_amount;
                            less_than_flag = 1;

                            //std::cerr<<"priciple_recoverable_before_date "<<priciple_recoverable_before_date<<" total_recoverable_before_date "<<total_recoverable_before_date;

                        }
                        if (it2->second->schedule_datum[it3].schedule_date > d_date_frm && it2->second->schedule_datum[it3].schedule_date <= d_date_to) {

                            priciple_recoverable_between_date = priciple_recoverable_between_date + it2->second->schedule_datum[it3].principle_installment_amount;
                            total_recoverable_between_date = total_recoverable_between_date + it2->second->schedule_datum[it3].installment_amount;

                            instalmanet_date.assign(boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it3].schedule_date).c_str());
                            installment_number = it3 + 1;

                            //std::cerr<<"priciple_recoverable_between_date "<<priciple_recoverable_between_date<<" total_recoverable_between_date "<<total_recoverable_between_date;
                            //std::cerr<<"instalmanet_date "<<instalmanet_date<<" installment_number "<<installment_number;

                        }
                        //std::cerr<<std::endl;
                    }
                    if (installment_number != it2->second->getFirstInstallmentNumber() - 1 || less_than_flag == 1) {
                        //std::cerr<<"installment_number != it2->second->getFirstInstallmentNumber()-1";
                        add_assoc_long(&subarray, "loan_id", it2->second->GetId());
                        add_assoc_long(&subarray, "member_id", it2->second->GetMember_id());
                        add_assoc_long(&subarray, "branch_id", it2->second->GetBranch_id());
                        add_assoc_string(&subarray, "samity_type", const_cast<char*> (samity_list[it2->second->GetSamity_id()]->GetSamity_type().c_str()));
                        add_assoc_long(&subarray, "samity_id", it2->second->GetSamity_id());
                        add_assoc_long(&subarray, "product_id", it2->second->GetProduct_id());
                        add_assoc_long(&subarray, "total_installment_number", it2->second->schedule_datum.size());
                        add_assoc_double(&subarray, "priciple_recoverable", priciple_recoverable);
                        add_assoc_double(&subarray, "total_recoverable", total_recoverable);

                        add_assoc_double(&subarray, "priciple_recoverable_between_date", priciple_recoverable_between_date);
                        add_assoc_double(&subarray, "total_recoverable_between_date", total_recoverable_between_date);

                        add_assoc_double(&subarray, "priciple_recoverable_before_date", priciple_recoverable_before_date);
                        add_assoc_double(&subarray, "total_recoverable_before_date", total_recoverable_before_date);

                        add_assoc_string(&subarray, "disbursement_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->GetDisburse_date()).c_str()));
                        add_assoc_string(&subarray, "first_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it2->second->getFirstInstallmentNumber()].schedule_date).c_str()));

                        add_assoc_string(&subarray, "last_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].schedule_date).c_str()));
                        add_assoc_string(&subarray, "instalmanet_date", const_cast<char*> (instalmanet_date.c_str()));
                        add_assoc_long(&subarray, "installment_number", installment_number);
                        add_assoc_string(&subarray, "loan_type", const_cast<char*>(it2->second->GetLoan_type().c_str()));
                        add_index_zval(&array1, it2->second->GetId(), &subarray);

                    } else if (it2->second->GetDisburse_date() <= d_date_to) {
                        //std::cerr<<"it2->second->GetDisburse_date()<= d_date_to";

                        //std::cerr<<" it2->second->GetId()"<<it2->second->GetId();
                        add_assoc_long(&subarray, "loan_id", it2->second->GetId());
                        add_assoc_long(&subarray, "member_id", it2->second->GetMember_id());
                        add_assoc_long(&subarray, "branch_id", it2->second->GetBranch_id());
                        add_assoc_string(&subarray, "samity_type", const_cast<char*> (samity_list[it2->second->GetSamity_id()]->GetSamity_type().c_str()));
                        add_assoc_long(&subarray, "samity_id", it2->second->GetSamity_id());
                        add_assoc_long(&subarray, "product_id", it2->second->GetProduct_id());
                        add_assoc_long(&subarray, "total_installment_number", it2->second->schedule_datum.size());
                        add_assoc_double(&subarray, "priciple_recoverable", priciple_recoverable);
                        add_assoc_double(&subarray, "total_recoverable", total_recoverable);

                        //std::cerr<<" priciple_recoverable_between_date"<<priciple_recoverable_between_date;

                        add_assoc_double(&subarray, "priciple_recoverable_between_date", priciple_recoverable_between_date);
                        //std::cerr<<" total_recoverable_between_date"<<total_recoverable_between_date;
                        add_assoc_double(&subarray, "total_recoverable_between_date", total_recoverable_between_date);

                        //std::cerr<<" priciple_recoverable_before_date"<<priciple_recoverable_before_date;                        
                        add_assoc_double(&subarray, "priciple_recoverable_before_date", priciple_recoverable_before_date);
                        //std::cerr<<" total_recoverable_before_date"<<total_recoverable_before_date;        
                        add_assoc_double(&subarray, "total_recoverable_before_date", total_recoverable_before_date);

                        //std::cerr<<" it2->second->getFirstInstallmentNumber()"<<it2->second->getFirstInstallmentNumber();
                        //std::cerr<<" first_schedule_date"<<it2->second->schedule_datum[it2->second->getFirstInstallmentNumber()].schedule_date;
                        //std::cerr<<" disbursement_date "<<it2->second->GetDisburse_date();                       
                        add_assoc_string(&subarray, "disbursement_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->GetDisburse_date()).c_str()));

                        //std::cerr<<" first_schedule_date"<<it2->second->schedule_datum[it2->second->getFirstInstallmentNumber()].schedule_date;                        
                        add_assoc_string(&subarray, "first_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it2->second->getFirstInstallmentNumber()].schedule_date).c_str()));

                        //std::cerr<<" it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].schedule_date"<<it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].schedule_date;
                        add_assoc_string(&subarray, "last_schedule_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->schedule_datum[it2->second->schedule_datum.size() - 1].schedule_date).c_str()));
                        add_assoc_string(&subarray, "instalmanet_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(it2->second->GetDisburse_date()).c_str()));
                        add_assoc_long(&subarray, "installment_number", installment_number);
                        add_assoc_string(&subarray, "loan_type", const_cast<char*>(it2->second->GetLoan_type().c_str()));
                        //std::cerr<<"it2->second->GetDisburse_date()<= d_date_to END END\n";
                        add_index_zval(&array1, it2->second->GetId(), &subarray);
                        //std::cerr<<"it2->second->GetDisburse_date()<= d_date_to END After add zindex\n";
                    }

                    //add_next_index_zval(array1, subarray);
                }
            }
        }
    }
    //std::cerr << "Before return" << std::endl;
    return array1;



}

void Loan_scheduler::group_by_branch_samity_product() {

}

zval Loan_scheduler::get_loan_principle_interrest_distribution_by_loan_id_trasaction_amount(int loan_id, double total_transaction_amount) {

    //std::cerr << "get_loan_principle_interrest_distribution_by_loan_id_trasaction_amount\n";

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    double total_installment = 0;
    double total_principle = 0;
    double total_interrest = 0;

    double last_installment = 0;
    double last_principle = 0;
    double last_interrest = 0;
    int last_installment_no = 0;
    std::vector<double> tmp = this->divide_transaction_to_installment_principle_interest_installment_no(loan_id, total_transaction_amount);
    total_installment = tmp[0];
    total_principle = tmp[1];
    total_interrest = tmp[2];
    last_installment_no = tmp[3];

    //std::cerr << total_installment << " " << total_principle << " " << total_interrest << " " << last_installment_no << std::endl;
    add_assoc_double(&array1, "installment_amount", total_installment);
    add_assoc_double(&array1, "principle_installment_amount", total_principle);
    add_assoc_double(&array1, "interrest_installment_amount", total_interrest);
    add_assoc_long(&array1, "last_fully_paid_installment_number", last_installment_no);
    return array1;
}

zval Loan_scheduler::get_loan_principle_interrest_distribution_by_loan_ids_trasaction_amounts(std::vector<int> loan_ids, std::vector<double> total_transaction_amounts) {

    //std::cerr << "get_loan_principle_interrest_distribution_by_loan_ids_trasaction_amounts\n";

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    for (int i = 0; i < loan_ids.size(); i++) {
        zval subarray;
//        MAKE_STD_ZVAL(subarray);
        array_init(&subarray);

        double total_installment = 0;
        double total_principle = 0;
        double total_interrest = 0;

        double last_installment = 0;
        double last_principle = 0;
        double last_interrest = 0;
        int last_installment_no = 0;
        //std::cerr << "Total Transaction Amounts " << total_transaction_amounts[i] << std::endl;
        //std::cerr << total_installment << " " << total_principle << " " << total_interrest << " " << this->loan_list[loan_ids[i]]->getFirstInstallmentNumber() << " " << this->loan_list[loan_ids[i]]->getLastInstallmentNumber() << std::endl;

        std::vector<double> tmp = this->divide_transaction_to_installment_principle_interest_installment_no(loan_ids[i], total_transaction_amounts[i]);
        total_installment = tmp[0];
        total_principle = tmp[1];
        total_interrest = tmp[2];
        last_installment_no = tmp[3];

        //std::cerr << total_installment << " " << total_principle << " " << total_interrest << " " << last_installment_no << std::endl;
        add_assoc_double(&subarray, "total_payable_amount", this->loan_list[loan_ids[i]]->GetTotal_payable_amount());
        add_assoc_string(&subarray, "first_repayment_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(this->loan_list[loan_ids[i]]->schedule_datum[0].schedule_date).c_str()));

        add_assoc_double(&subarray, "installment_amount", total_installment);
        add_assoc_double(&subarray, "principle_installment_amount", total_principle);
        add_assoc_double(&subarray, "interrest_installment_amount", total_interrest);
        add_assoc_long(&subarray, "last_fully_paid_installment_number", last_installment_no);
        add_next_index_zval(&array1, &subarray);
    }
    return array1;
}

zval Loan_scheduler::get_loan_advance_due_by_loan_ids_trasaction_amounts(std::vector<int> loan_ids, std::vector<double> total_transaction_amounts, std::string s_date, bool include_expired = false) {

    //std::cerr << "get_loan_advance_due_by_loan_ids_trasaction_amounts\n";
    //std::cerr << "s_date "<<s_date<<std::endl;
    boost::gregorian::date d_date = boost::gregorian::from_simple_string(s_date);
    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);
    //return array1;

    zval subarray;
//    MAKE_STD_ZVAL(subarray);
    array_init(&subarray);

    double advance = 0;
    double due = 0;
    double total_payable = 0;
    double total_paid = 0;

    double total_extra_paid = 0;

    for (int i = 0; i < loan_ids.size(); i++) {
        //std::cerr<< "loan_id "<<loan_ids[i]<<" total_transaction_amounts "<<total_transaction_amounts[i]<<" Date "<<s_date<<std::endl;
        total_extra_paid = total_extra_paid + total_transaction_amounts[i];

        if (this->loan_list[loan_ids[i]]->GetLoan_fully_paid_date() >= d_date) {
            if (include_expired == false && this->loan_list[loan_ids[i]]->GetLast_repayment_date() < d_date) {
                continue;
            }
            double loan_advance = 0;
            double loan_due = 0;
            double loan_total_payable = 0;
            double loan_total_paid = total_transaction_amounts[i];

            //std::cerr<< "loan_id "<<loan_ids[i]<<" total_transaction_amounts "<<total_transaction_amounts[i]<<" Date "<<s_date<<std::endl;
            for (int index = this->loan_list[loan_ids[i]]->getFirstInstallmentNumber(); index<this->loan_list[loan_ids[i]]->getLastInstallmentNumber(); index++) {
                if (this->loan_list[loan_ids[i]]->schedule_datum[index].schedule_date < d_date) {
                    loan_total_payable = loan_total_payable + this->loan_list[loan_ids[i]]->schedule_datum[index].installment_amount;
                } else {
                    break;
                }

            }
            if (loan_total_paid >= loan_total_payable) {
                loan_advance = loan_total_paid - loan_total_payable;
            } else {
                loan_due = loan_total_payable - loan_total_paid;
            }
            advance = advance + loan_advance;
            due = due + loan_due;
            total_payable = total_payable + loan_total_payable;
            total_paid = total_paid + loan_total_paid;

            zval subarray1;
//            MAKE_STD_ZVAL(subarray1);
            array_init(&subarray1);

            add_assoc_double(&subarray1, "advance", advance);
            add_assoc_double(&subarray1, "due", due);
            add_assoc_double(&subarray1, "total_payable", total_payable);
            add_assoc_long(&subarray1, "total_paid", total_paid);
            add_assoc_string(&subarray1, "transfer_in_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(this->loan_list[loan_ids[i]]->GetTransfer_in_date()).c_str()));
            add_assoc_string(&subarray1, "transfer_out_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(this->loan_list[loan_ids[i]]->GetTransfer_out_date()).c_str()));

            //std::cerr<< "loan_id "<<loan_ids[i]<<" total_transaction_amounts "<<total_transaction_amounts[i]<<" Date "<<s_date<<std::endl;

            add_index_zval(&subarray, loan_ids[i], &subarray1);
        } else {

            advance = advance + 0;
            due = due + 0;
            total_payable = total_payable + this->loan_list[loan_ids[i]]->GetTotal_payable_amount();
            total_paid = total_paid + this->loan_list[loan_ids[i]]->GetTotal_payable_amount();

            zval subarray1;
//            MAKE_STD_ZVAL(subarray1);
            array_init(&subarray1);

            add_assoc_double(&subarray1, "advance", advance);
            add_assoc_double(&subarray1, "due", due);
            add_assoc_double(&subarray1, "total_payable", total_payable);
            add_assoc_long(&subarray1, "total_paid", total_paid);
            add_assoc_string(&subarray1, "transfer_in_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(this->loan_list[loan_ids[i]]->GetTransfer_in_date()).c_str()));
            add_assoc_string(&subarray1, "transfer_out_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(this->loan_list[loan_ids[i]]->GetTransfer_out_date()).c_str()));
        }


    }
    total_extra_paid = total_extra_paid - total_paid;
    add_assoc_double(&array1, "advance", advance);
    add_assoc_double(&array1, "due", due);
    add_assoc_double(&array1, "total_payable", total_payable);
    add_assoc_long(&array1, "total_paid", total_paid);
    add_assoc_long(&array1, "total_extra_paid", total_extra_paid);

    //std::cerr<<"before end\n";
    add_assoc_zval(&array1, "loans", &subarray);
    //std::cerr<<"after end\n";

    return array1;
}

zval Loan_scheduler::get_loan_advance_due_by_loan_ids_trasaction_principal_amounts(std::vector<int> loan_ids, std::vector<double> total_transaction_principal_amounts, std::string s_date, bool include_expired = false) {

    //std::cerr << "get_loan_advance_due_by_loan_ids_trasaction_amounts\n";
    //std::cerr << "s_date "<<s_date<<std::endl;
    boost::gregorian::date d_date = boost::gregorian::from_simple_string(s_date);
    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);
    //return array1;

    zval subarray;
//    MAKE_STD_ZVAL(subarray);
    array_init(&subarray);

    double advance = 0;
    double due = 0;
    double total_payable = 0;
    double total_paid = 0;

    double total_extra_paid = 0;

    for (int i = 0; i < loan_ids.size(); i++) {
        //std::cerr<< "loan_id "<<loan_ids[i]<<" total_transaction_amounts "<<total_transaction_amounts[i]<<" Date "<<s_date<<std::endl;
        total_extra_paid = total_extra_paid + total_transaction_principal_amounts[i];

        if (this->loan_list[loan_ids[i]]->GetLoan_fully_paid_date() >= d_date) {
            if (include_expired == false && this->loan_list[loan_ids[i]]->GetLast_repayment_date() < d_date) {
                continue;
            }
            double loan_advance = 0;
            double loan_due = 0;
            double loan_total_payable = 0;
            double loan_total_paid = total_transaction_principal_amounts[i];

            //std::cerr<< "loan_id "<<loan_ids[i]<<" total_transaction_amounts "<<total_transaction_amounts[i]<<" Date "<<s_date<<std::endl;
            for (int index = this->loan_list[loan_ids[i]]->getFirstInstallmentNumber(); index<this->loan_list[loan_ids[i]]->getLastInstallmentNumber(); index++) {
                if (this->loan_list[loan_ids[i]]->schedule_datum[index].schedule_date < d_date) {
                    loan_total_payable = loan_total_payable + this->loan_list[loan_ids[i]]->schedule_datum[index].principle_installment_amount;
                } else {
                    break;
                }

            }
            if (loan_total_paid >= loan_total_payable) {
                loan_advance = loan_total_paid - loan_total_payable;
            } else {
                loan_due = loan_total_payable - loan_total_paid;
            }
            advance = advance + loan_advance;
            due = due + loan_due;
            total_payable = total_payable + loan_total_payable;
            total_paid = total_paid + loan_total_paid;

            zval subarray1;
//            MAKE_STD_ZVAL(subarray1);
            array_init(&subarray1);

            add_assoc_double(&subarray1, "principal_advance", advance);
            add_assoc_double(&subarray1, "principal_due", due);
            add_assoc_double(&subarray1, "principal_total_payable", total_payable);
            add_assoc_long(&subarray1, "principal_total_paid", total_paid);

            //std::cerr<< "loan_id "<<loan_ids[i]<<" total_transaction_amounts "<<total_transaction_amounts[i]<<" Date "<<s_date<<std::endl;

            add_index_zval(&subarray, loan_ids[i], &subarray1);
        } else {

            advance = advance + 0;
            due = due + 0;
            total_payable = total_payable + this->loan_list[loan_ids[i]]->GetTotal_payable_amount();
            total_paid = total_paid + this->loan_list[loan_ids[i]]->GetTotal_payable_amount();

            zval subarray1;
//            MAKE_STD_ZVAL(subarray1);
            array_init(&subarray1);

            add_assoc_double(&subarray1, "principal_advance", advance);
            add_assoc_double(&subarray1, "principal_due", due);
            add_assoc_double(&subarray1, "principal_total_payable", total_payable);
            add_assoc_long(&subarray1, "principal_total_paid", total_paid);
        }


    }
    total_extra_paid = total_extra_paid - total_paid;
    add_assoc_double(&array1, "principal_advance", advance);
    add_assoc_double(&array1, "principal_due", due);
    add_assoc_double(&array1, "principal_total_payable", total_payable);
    add_assoc_long(&array1, "principal_total_paid", total_paid);

    //std::cerr<<"before end\n";
    add_assoc_zval(&array1, "loans", &subarray);
    //std::cerr<<"after end\n";

    return array1;
}

zval Loan_scheduler::get_loan_advance_due_by_loan_ids_trasaction_amounts_between_date(std::vector<int> loan_ids, std::vector<double> total_transaction_before_from_date, std::vector<double> total_transaction_before_to_date, std::string s_date_from, std::string s_date_to, bool include_expired, std::vector<double> principle_total_transaction_before_from_date, std::vector<double> interest_total_transaction_before_from_date, std::vector<double> principle_total_transaction_before_to_date, std::vector<double> interest_total_transaction_before_to_date) {

    //std::cerr << "get_loan_advance_due_by_loan_ids_trasaction_amounts_between_date\n";
    //std::cerr << "s_date_from "<<s_date_from<<"s_date_to "<<s_date_to<<std::endl;
    boost::gregorian::date d_date_from = boost::gregorian::from_simple_string(s_date_from);
    boost::gregorian::date d_date_to = boost::gregorian::from_simple_string(s_date_to);

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    zval subarray;
//    MAKE_STD_ZVAL(subarray);
    array_init(&subarray);

    double advance = 0;
    double principle_advance = 0;
    double interest_advance = 0;
    double due = 0;
    double principle_due = 0;
    double interest_due = 0;
    double total_payable = 0;
    double principle_total_payable = 0;
    double interest_total_payable = 0;
    double total_paid = 0;
    double principle_total_paid = 0;
    double interest_total_paid = 0;
    double advance_recovery = 0;
    double principle_advance_recovery = 0;
    double interest_advance_recovery = 0;
    double due_recovery = 0;
    double principle_due_recovery = 0;
    double interest_due_recovery = 0;

    double total_extra_paid = 0;

    double regular_recovery = 0;
    double principle_regular_recovery = 0;
    double interest_regular_recovery = 0;


    for (int i = 0; i < loan_ids.size(); i++) {

        total_extra_paid = total_extra_paid + total_transaction_before_to_date[i] - total_transaction_before_from_date[i];

        if (this->loan_list[loan_ids[i]]->GetLoan_fully_paid_date() >= d_date_from) {
            if (include_expired == false && this->loan_list[loan_ids[i]]->GetLast_repayment_date() < d_date_from) {
                continue;
            }
            double loan_advance = 0;
            double principle_loan_advance = 0;
            double interest_loan_advance = 0;
            double loan_due = 0;
            double principle_loan_due = 0;
            double interest_loan_due = 0;
            double loan_total_payable = 0;
            double principle_loan_total_payable = 0;
            double interest_loan_total_payable = 0;
            double loan_total_paid = total_transaction_before_from_date[i];
            double principle_loan_total_paid = 0;
            double interest_loan_total_paid = 0;
            double loan_advance_recovery = 0;
            double principle_loan_advance_recovery = 0;
            double interest_loan_advance_recovery = 0;
            double loan_due_recovery = 0;
            double principle_loan_due_recovery = 0;
            double interest_loan_due_recovery = 0;

            double loan_advance_before_from_date = 0;
            double principle_loan_advance_before_from_date = 0;
            double interest_loan_advance_before_from_date = 0;
            double loan_due_before_from_date = 0;
            double principle_loan_due_before_from_date = 0;
            double interest_loan_due_before_from_date = 0;
            double loan_total_payable_before_from_date = 0;
            double principle_loan_total_payable_before_from_date = 0;
            double interest_loan_total_payable_before_from_date = 0;
            double loan_total_paid_before_from_date = total_transaction_before_from_date[i];
            double principle_loan_total_paid_before_from_date = 0;
            double interest_loan_total_paid_before_from_date = 0;

            double loan_advance_before_to_date = 0;
            double principle_loan_advance_before_to_date = 0;
            double interest_loan_advance_before_to_date = 0;
            double loan_due_before_to_date = 0;
            double principle_loan_due_before_to_date = 0;
            double interest_loan_due_before_to_date = 0;
            double loan_total_payable_before_to_date = 0;
            double principle_loan_total_payable_before_to_date = 0;
            double interest_loan_total_payable_before_to_date = 0;
            double loan_total_paid_before_to_date = total_transaction_before_to_date[i];
            double principle_loan_total_paid_before_to_date = 0;
            double interest_loan_total_paid_before_to_date = 0;

            std::vector<double> tmp;

            if (principle_total_transaction_before_from_date.size() <= i + 1) {

                if (total_transaction_before_from_date[i] == 0) {
                    principle_loan_total_paid = 0;
                    interest_loan_total_paid = 0;

                    principle_loan_total_paid_before_from_date = 0;
                    interest_loan_total_paid_before_from_date = 0;
                } else {
                    tmp = this->divide_transaction_to_installment_principle_interest_installment_no(loan_ids[i], total_transaction_before_from_date[i]);

                    principle_loan_total_paid = tmp[1];
                    interest_loan_total_paid = tmp[2];

                    principle_loan_total_paid_before_from_date = tmp[1];
                    interest_loan_total_paid_before_from_date = tmp[2];
                }
            } else {
                principle_loan_total_paid = principle_total_transaction_before_from_date[i];
                interest_loan_total_paid = interest_total_transaction_before_from_date[i];

                principle_loan_total_paid_before_from_date = principle_total_transaction_before_from_date[i];
                interest_loan_total_paid_before_from_date = interest_total_transaction_before_from_date[i];
            }
            if (principle_total_transaction_before_to_date.size() <= i + 1) {

                if (total_transaction_before_to_date[i] == 0) {
                    principle_loan_total_paid_before_to_date = 0;
                    interest_loan_total_paid_before_to_date = 0;
                } else {
                    if (this->loan_list[loan_ids[i]]->GetLoan_type() == "O" && this->loan_list[loan_ids[i]]->GetRepayment_frequency() == "M") {
                        principle_loan_total_paid_before_to_date = principle_total_transaction_before_to_date[i];
                        interest_loan_total_paid_before_to_date = interest_total_transaction_before_to_date[i];
                    } else {
                        tmp = this->divide_transaction_to_installment_principle_interest_installment_no(loan_ids[i], total_transaction_before_to_date[i]);
                        principle_loan_total_paid_before_to_date = tmp[1];
                        interest_loan_total_paid_before_to_date = tmp[2];
                        }

                }
            } else {
                principle_loan_total_paid_before_to_date = principle_total_transaction_before_to_date[i];
                interest_loan_total_paid_before_to_date = interest_total_transaction_before_to_date[i];
            }
            
            for (int index = this->loan_list[loan_ids[i]]->getFirstInstallmentNumber(); index<this->loan_list[loan_ids[i]]->getLastInstallmentNumber(); index++) {
                if (this->loan_list[loan_ids[i]]->schedule_datum[index].schedule_date < d_date_from) {
                    loan_total_payable_before_from_date += this->loan_list[loan_ids[i]]->schedule_datum[index].installment_amount;
                    principle_loan_total_payable_before_from_date += this->loan_list[loan_ids[i]]->schedule_datum[index].principle_installment_amount;
                    interest_loan_total_payable_before_from_date += this->loan_list[loan_ids[i]]->schedule_datum[index].interrest_installment_amount;
                } else {
                    break;
                }

            }
            if (loan_total_paid_before_from_date >= loan_total_payable_before_from_date) {
                loan_advance_before_from_date = loan_total_paid_before_from_date - loan_total_payable_before_from_date;
                principle_loan_advance_before_from_date = principle_loan_total_paid_before_from_date - principle_loan_total_payable_before_from_date;
                interest_loan_advance_before_from_date = interest_loan_total_paid_before_from_date - interest_loan_total_payable_before_from_date;
            } else {
                loan_due_before_from_date = loan_total_payable_before_from_date - loan_total_paid_before_from_date;
                principle_loan_due_before_from_date = principle_loan_total_payable_before_from_date - principle_loan_total_paid_before_from_date;
                interest_loan_due_before_from_date = interest_loan_total_payable_before_from_date - interest_loan_total_paid_before_from_date;
            }

            for (int index = this->loan_list[loan_ids[i]]->getFirstInstallmentNumber(); index<this->loan_list[loan_ids[i]]->getLastInstallmentNumber(); index++) {
                if (this->loan_list[loan_ids[i]]->schedule_datum[index].schedule_date <= d_date_to) {
                    loan_total_payable_before_to_date += this->loan_list[loan_ids[i]]->schedule_datum[index].installment_amount;
                    principle_loan_total_payable_before_to_date += this->loan_list[loan_ids[i]]->schedule_datum[index].principle_installment_amount;
                    interest_loan_total_payable_before_to_date += this->loan_list[loan_ids[i]]->schedule_datum[index].interrest_installment_amount;
                } else {
                    break;
                }
            }
            //std::cerr<<'loan_id '<<this->loan_list[loan_ids[i]]->GetId()<<std::endl;
            //std::cerr<<"loan_total_payable_before_to_date "<<loan_total_payable_before_to_date<<std::endl;
            //std::cerr<<"principle_loan_total_payable_before_to_date "<<principle_loan_total_payable_before_to_date<<std::endl;
            //std::cerr<<"interest_loan_total_payable_before_to_date "<<interest_loan_total_payable_before_to_date<<std::endl;
            //loan_total_payable_before_to_date=(loan_total_payable_before_to_date-loan_advance_before_from_date>0?loan_total_payable_before_to_date-loan_advance_before_from_date:0);

            if (loan_total_paid_before_to_date >= loan_total_payable_before_to_date) {
                loan_advance_before_to_date = loan_total_paid_before_to_date - loan_total_payable_before_to_date;
                principle_loan_advance_before_to_date = principle_loan_total_paid_before_to_date - principle_loan_total_payable_before_to_date;
                interest_loan_advance_before_to_date = interest_loan_total_paid_before_to_date - interest_loan_total_payable_before_to_date;
            } else {
                loan_due_before_to_date = loan_total_payable_before_to_date - loan_total_paid_before_to_date;
                principle_loan_due_before_to_date = principle_loan_total_payable_before_to_date - principle_loan_total_paid_before_to_date;
                interest_loan_due_before_to_date = interest_loan_total_payable_before_to_date - interest_loan_total_paid_before_to_date;
            }         
            //std::cerr<<"Principal payable Due "<<principle_loan_total_payable_before_to_date<<" Loan Id  "<< this->loan_list[loan_ids[i]]->GetId()<<std::endl;
            //std::cerr<<"Principal paid Due "<<principle_loan_total_paid_before_to_date<<" Loan Id  "<< this->loan_list[loan_ids[i]]->GetId()<<std::endl;
            loan_advance = loan_advance_before_to_date;
            principle_loan_advance = principle_loan_advance_before_to_date;
            interest_loan_advance = interest_loan_advance_before_to_date;
            loan_due = loan_due_before_to_date;
            //std::cerr<<"Principle Due "<<principle_loan_due_before_to_date<<" Loan Id  "<< this->loan_list[loan_ids[i]]->GetId()<<std::endl;
            principle_loan_due = principle_loan_due_before_to_date;
            interest_loan_due = interest_loan_due_before_to_date;
            loan_total_payable = (loan_total_payable_before_to_date - loan_total_payable_before_from_date - loan_advance_before_from_date > 0 ? loan_total_payable_before_to_date - loan_total_payable_before_from_date - loan_advance_before_from_date : 0);
            principle_loan_total_payable = (principle_loan_total_payable_before_to_date - principle_loan_total_payable_before_from_date - principle_loan_advance_before_from_date > 0 ? principle_loan_total_payable_before_to_date - principle_loan_total_payable_before_from_date - principle_loan_advance_before_from_date : 0);
            interest_loan_total_payable = (interest_loan_total_payable_before_to_date - interest_loan_total_payable_before_from_date - interest_loan_advance_before_from_date > 0 ? interest_loan_total_payable_before_to_date - interest_loan_total_payable_before_from_date - interest_loan_advance_before_from_date : 0);

            loan_total_paid = loan_total_paid_before_to_date - loan_total_paid_before_from_date;
            principle_loan_total_paid = principle_loan_total_paid_before_to_date - principle_loan_total_paid_before_from_date;
            interest_loan_total_paid = interest_loan_total_paid_before_to_date - interest_loan_total_paid_before_from_date;

            if (loan_total_paid > loan_total_payable) {
                if (loan_total_paid >= loan_advance_before_to_date) {
                    loan_advance_recovery = loan_advance_before_to_date;
                    principle_loan_advance_recovery = principle_loan_advance_before_to_date;
                    interest_loan_advance_recovery = interest_loan_advance_before_to_date;
                } else {
                    loan_advance_recovery = loan_total_paid;
                    principle_loan_advance_recovery = principle_loan_total_paid;
                    interest_loan_advance_recovery = interest_loan_total_paid;
                }
            }

            


            //loan_due_recovery = (loan_due_before_from_date-loan_due_before_to_date>0?loan_due_before_from_date-loan_due_before_to_date:0);
            loan_due_recovery = (loan_due_before_from_date - loan_due_before_to_date > 0 ? loan_due_before_from_date - loan_due_before_to_date : 0);
            principle_loan_due_recovery = (principle_loan_due_before_from_date - principle_loan_due_before_to_date > 0 ? principle_loan_due_before_from_date - principle_loan_due_before_to_date : 0);
            interest_loan_due_recovery = (interest_loan_due_before_from_date - interest_loan_due_before_to_date > 0 ? interest_loan_due_before_from_date - interest_loan_due_before_to_date : 0);

            
            double loan_regular_recovery = (loan_total_paid - loan_advance_recovery - loan_due_recovery) > 0 ? (loan_total_paid - loan_advance_recovery - loan_due_recovery) : 0;
            double principle_loan_regular_recovery = (principle_loan_total_paid - principle_loan_advance_recovery - principle_loan_due_recovery) > 0 ? (principle_loan_total_paid - principle_loan_advance_recovery - principle_loan_due_recovery) : 0;
            double interest_loan_regular_recovery = (interest_loan_total_paid - interest_loan_advance_recovery - interest_loan_due_recovery) > 0 ? (interest_loan_total_paid - interest_loan_advance_recovery - interest_loan_due_recovery) : 0;


            if (this->loan_list[loan_ids[i]]->GetInterest_calculation_method().compare("DFD") != 0) {
                loan_advance = _round(loan_advance);
                principle_loan_advance = _round(principle_loan_advance);
                interest_loan_advance = _round(interest_loan_advance);
                loan_due = _round(loan_due);
                principle_loan_due = _round(principle_loan_due);
                interest_loan_due = _round(interest_loan_due);
                loan_total_payable = _round(loan_total_payable);
                principle_loan_total_payable = _round(principle_loan_total_payable);
                interest_loan_total_payable = _round(interest_loan_total_payable);
                loan_total_paid = _round(loan_total_paid);
                principle_loan_total_paid = _round(principle_loan_total_paid);
                interest_loan_total_paid = _round(interest_loan_total_paid);
                loan_advance_recovery = _round(loan_advance_recovery);
                principle_loan_advance_recovery = _round(principle_loan_advance_recovery);
                interest_loan_advance_recovery = _round(interest_loan_advance_recovery);
                loan_due_recovery = _round(loan_due_recovery);
                principle_loan_due_recovery = _round(principle_loan_due_recovery);
                interest_loan_due_recovery = _round(interest_loan_due_recovery);

                loan_regular_recovery = _round(loan_regular_recovery);
                principle_loan_regular_recovery = _round(principle_loan_regular_recovery);
                interest_loan_regular_recovery = _round(interest_loan_regular_recovery);
            } else {
                loan_advance = _round10(loan_advance);
                principle_loan_advance = _round10(principle_loan_advance);
                interest_loan_advance = _round10(interest_loan_advance);
                loan_due = _round10(loan_due);
                principle_loan_due = _round10(principle_loan_due);
                interest_loan_due = _round10(interest_loan_due);
                loan_total_payable = _round10(loan_total_payable);
                principle_loan_total_payable = _round10(principle_loan_total_payable);
                interest_loan_total_payable = _round10(interest_loan_total_payable);
                loan_total_paid = _round10(loan_total_paid);
                principle_loan_total_paid = _round10(principle_loan_total_paid);
                interest_loan_total_paid = _round10(interest_loan_total_paid);
                loan_advance_recovery = _round10(loan_advance_recovery);
                principle_loan_advance_recovery = _round10(principle_loan_advance_recovery);
                interest_loan_advance_recovery = _round10(interest_loan_advance_recovery);
                loan_due_recovery = _round10(loan_due_recovery);
                principle_loan_due_recovery = _round10(principle_loan_due_recovery);
                interest_loan_due_recovery = _round10(interest_loan_due_recovery);

                loan_regular_recovery = _round10(loan_regular_recovery);
                principle_loan_regular_recovery = _round10(principle_loan_regular_recovery);
                interest_loan_regular_recovery = _round10(interest_loan_regular_recovery);
            }

            advance = advance + loan_advance;
            principle_advance = principle_advance + principle_loan_advance;
            interest_advance = interest_advance + interest_loan_advance;
            due = due + loan_due;
            principle_due = principle_due + principle_loan_due;
            interest_due = interest_due + interest_loan_due;
            total_payable = total_payable + loan_total_payable;
            principle_total_payable = principle_total_payable + principle_loan_total_payable;
            interest_total_payable = interest_total_payable + interest_loan_total_payable;
            total_paid = total_paid + loan_total_paid;
            principle_total_paid = principle_total_paid + principle_loan_total_paid;
            interest_total_paid = interest_total_paid + interest_loan_total_paid;

            regular_recovery += loan_regular_recovery;
            principle_regular_recovery += principle_loan_regular_recovery;
            interest_regular_recovery += interest_loan_regular_recovery;

            advance_recovery += loan_advance_recovery;
            principle_advance_recovery += principle_loan_advance_recovery;
            interest_advance_recovery += interest_loan_advance_recovery;
            due_recovery += loan_due_recovery;
            principle_due_recovery += principle_loan_due_recovery;
            interest_due_recovery += interest_loan_due_recovery;


            zval subarray1;
//            MAKE_STD_ZVAL(subarray1);
            array_init(&subarray1);

            add_assoc_double(&subarray1, "advance", loan_advance);
            add_assoc_double(&subarray1, "principle_advance", principle_loan_advance);
            add_assoc_double(&subarray1, "interest_advance", interest_loan_advance);
            add_assoc_double(&subarray1, "due", loan_due);
            add_assoc_double(&subarray1, "principle_due", principle_loan_due);
            add_assoc_double(&subarray1, "interest_due", interest_loan_due);
            add_assoc_double(&subarray1, "total_payable", loan_total_payable);
            add_assoc_double(&subarray1, "principle_total_payable", principle_loan_total_payable);
            add_assoc_double(&subarray1, "interest_total_payable", interest_loan_total_payable);
            add_assoc_double(&subarray1, "total_paid", loan_total_paid);
            add_assoc_double(&subarray1, "principle_total_paid", principle_loan_total_paid);
            add_assoc_double(&subarray1, "interest_total_paid", interest_loan_total_paid);

            add_assoc_double(&subarray1, "regular_recovery", loan_regular_recovery);
            add_assoc_double(&subarray1, "principle_regular_recovery", principle_loan_regular_recovery);
            add_assoc_double(&subarray1, "interest_regular_recovery", interest_loan_regular_recovery);

            add_assoc_double(&subarray1, "advance_recovery", loan_advance_recovery);
            add_assoc_double(&subarray1, "principle_advance_recovery", principle_loan_advance_recovery);
            add_assoc_double(&subarray1, "interest_advance_recovery", interest_loan_advance_recovery);

            add_assoc_double(&subarray1, "due_recovery", loan_due_recovery);
            add_assoc_double(&subarray1, "principle_due_recovery", principle_loan_due_recovery);
            add_assoc_double(&subarray1, "interest_due_recovery", interest_loan_due_recovery);

            add_assoc_string(&subarray1, "transfer_in_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(this->loan_list[loan_ids[i]]->GetTransfer_in_date()).c_str()));
            add_assoc_string(&subarray1, "transfer_out_date", const_cast<char*> (boost::gregorian::to_iso_extended_string(this->loan_list[loan_ids[i]]->GetTransfer_out_date()).c_str()));

            add_index_zval(&subarray, loan_ids[i], &subarray1);
        }
        /*
        else{
            
            advance = advance + 0;
            due = due + 0;
            total_payable = total_payable + this->loan_list[loan_ids[i]]->GetTotal_payable_amount();
            total_paid = total_paid + this->loan_list[loan_ids[i]]->GetTotal_payable_amount();
            
            zval *subarray1;
            MAKE_STD_ZVAL(subarray1);
            array_init(subarray1);
            
            add_assoc_double(subarray1, "advance", advance);
            add_assoc_double(subarray1, "due", due);
            add_assoc_double(subarray1, "total_payable", total_payable);
            add_assoc_long(subarray1, "total_paid", total_paid);
            add_assoc_double(subarray1, "advance_recovery", advance_recovery);
            add_assoc_long(subarray1, "due_recovery", due_recovery);
            
            add_index_zval(subarray, loan_ids[i], subarray1);
        }
         */


    }
    total_extra_paid = total_extra_paid - total_paid;

    add_assoc_double(&array1, "advance", advance);
    add_assoc_double(&array1, "principle_advance", principle_advance);
    add_assoc_double(&array1, "interest_advance", interest_advance);
    add_assoc_double(&array1, "due", due);
    add_assoc_double(&array1, "principle_due", principle_due);
    add_assoc_double(&array1, "interest_due", interest_due);
    add_assoc_double(&array1, "total_payable", total_payable);
    add_assoc_double(&array1, "principle_total_payable", principle_total_payable);
    add_assoc_double(&array1, "interest_total_payable", interest_total_payable);

    add_assoc_double(&array1, "total_regular_recovery", regular_recovery);
    add_assoc_double(&array1, "principle_total_regular_recovery", principle_regular_recovery);
    add_assoc_double(&array1, "interest_total_regular_recovery", interest_regular_recovery);

    add_assoc_double(&array1, "total_advance_recovery", advance_recovery);
    add_assoc_double(&array1, "principle_total_advance_recovery", principle_advance_recovery);
    add_assoc_double(&array1, "interest_total_advance_recovery", interest_advance_recovery);

    add_assoc_double(&array1, "total_due_recovery", due_recovery);
    add_assoc_double(&array1, "principle_total_due_recovery", principle_due_recovery);
    add_assoc_double(&array1, "interest_total_due_recovery", interest_due_recovery);
    add_assoc_double(&array1, "total_paid", total_paid);
    add_assoc_double(&array1, "principle_total_paid", principle_total_paid);
    add_assoc_double(&array1, "interest_total_paid", interest_total_paid);
    add_assoc_double(&array1, "total_extra_paid", total_extra_paid);

    add_assoc_zval(&array1, "loans", &subarray);

    return array1;
}

zval Loan_scheduler::get_max_penalty_by_loan_ids(std::vector<int> loan_ids) {

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);
    for (int i = 0; i < loan_ids.size(); i++) {
        zval subarray;
//        MAKE_STD_ZVAL(subarray);
        array_init(&subarray);

        add_assoc_long(&subarray, "loan_id", this->loan_list[loan_ids[i]]->GetId());
        add_assoc_double(&subarray, "max_penalty", this->loan_list[loan_ids[i]]->schedule_datum[this->loan_list[loan_ids[i]]->schedule_datum.size() - 1].interrest_installment_amount);
        add_next_index_zval(&array1, &subarray);

    }

    return array1;
}

zval Loan_scheduler::get_max_rebate_by_loan_ids(std::vector<int> loan_ids, std::vector<int> no_of_installment_paid) {

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);
    for (int i = 0; i < loan_ids.size(); i++) {
        double rebate = 0;
        for (int index = no_of_installment_paid[i]; index<this->loan_list[loan_ids[i]]->schedule_datum.size(); index++) {
            rebate = rebate + this->loan_list[loan_ids[i]]->schedule_datum[index].interrest_installment_amount;
        }
        zval subarray;
//        MAKE_STD_ZVAL(subarray);
        array_init(&subarray);

        add_assoc_long(&subarray, "loan_id", this->loan_list[loan_ids[i]]->GetId());
        add_assoc_double(&subarray, "max_rebate", rebate);
        add_next_index_zval(&array1, &subarray);

    }

    return array1;
}

zval Loan_scheduler::get_max_waiver_by_loan_ids(std::vector<int> loan_ids, std::vector<int> no_of_installment_paid) {

    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);
    for (int i = 0; i < loan_ids.size(); i++) {
        double waiver = 0;
        for (int index = no_of_installment_paid[i]; index<this->loan_list[loan_ids[i]]->schedule_datum.size(); index++) {
            waiver = waiver + this->loan_list[loan_ids[i]]->schedule_datum[index].installment_amount;
        }
        zval subarray;
//        MAKE_STD_ZVAL(subarray);
        array_init(&subarray);

        add_assoc_long(&subarray, "loan_id", this->loan_list[loan_ids[i]]->GetId());
        add_assoc_double(&subarray, "max_waiver", waiver);
        add_next_index_zval(&array1, &subarray);

    }

    return array1;
}

std::vector<double> Loan_scheduler::divide_transaction_to_installment_principle_interest_installment_no(int loan_id, double total_transaction_amount) {

    //std::cerr << "divide_transaction_to_installment_principle_interest_installment_no\n";
    //std::cerr<<"Parameters Loan_id "<<loan_id<<" Total Transaction Amount "<<total_transaction_amount<<"\n";

    //std::cerr<<this->loan_list[loan_id]->GetId()<<std::endl;
    //std::cerr<<this->loan_list[loan_id]->GetLoan_type()<<std::endl;
    //std::cerr<<this->loan_list[loan_id]->GetRepayment_frequency()<<std::endl;
    //total_transaction_amount=_round(total_transaction_amount);

    if (total_transaction_amount == 0) {
        std::vector<double> installment_principle_interest_installment_no;
        installment_principle_interest_installment_no.push_back(0);
        installment_principle_interest_installment_no.push_back(0);
        installment_principle_interest_installment_no.push_back(0);
        installment_principle_interest_installment_no.push_back(0);

        return installment_principle_interest_installment_no;
    }
    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    double total_installment = 0;
    double total_principle = 0;
    double total_interrest = 0;

    double last_installment = 0;
    double last_principle = 0;
    double last_interrest = 0;
    int last_fully_paid_installment_no = 0;

    double last_installment_no = 0;
    if (this->loan_list[loan_id]->GetLoan_type().compare("O") == 0 && (this->loan_list[loan_id]->GetRepayment_frequency().compare("W") == 0 || this->loan_list[loan_id]->GetRepayment_frequency().compare("M") == 0)) {
        total_installment = 0;
        total_principle = 0;
        total_interrest = 0;
        last_fully_paid_installment_no = 0;
        //std::cerr<<"One Time Division\n";

        for (int i = this->loan_list[loan_id]->getFirstInstallmentNumber(); i<this->loan_list[loan_id]->getLastInstallmentNumber(); i++) {
            if (total_transaction_amount > total_installment) {
                total_principle += this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                total_interrest += this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                total_installment = total_principle + total_interrest;
                last_fully_paid_installment_no = i;
            }
            if (total_transaction_amount == total_installment && total_transaction_amount > 0) {
                last_fully_paid_installment_no = i;
                break;
            } else if (total_transaction_amount < total_installment) {
                last_fully_paid_installment_no = (i - 1 > 0 ? i - 1 : 0);

                total_principle -= this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                total_interrest -= this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                total_installment = total_principle + total_interrest;

                //total_principle+=this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                if (total_installment + this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount <= total_transaction_amount) {
                    total_interrest += this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                    total_installment = total_principle + total_interrest;
                }
                if (total_installment + this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount <= total_transaction_amount) {
                    total_principle += this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                    total_installment = total_principle + total_interrest;
                } else {
                    total_principle += total_transaction_amount - total_installment;
                    total_installment = total_principle + total_interrest;
                }
                break;

            }
            //std::cerr<<"Divide return "<<total_installment<<" "<<total_principle<<" "<<total_interrest<<" "<<last_fully_paid_installment_no<<"\n";
        }

        if (total_transaction_amount > total_installment) {
            total_interrest += total_transaction_amount - total_installment;
            total_installment = total_principle + total_interrest;
        }
        //std::cerr<<"Divide return "<<total_installment<<" "<<total_principle<<" "<<total_interrest<<" "<<last_fully_paid_installment_no<<"\n";
        //exit(1);
    } else if (this->loan_list[loan_id]->GetLoan_type().compare("O") == 0) {
        total_installment = 0;
        total_principle = 0;
        total_interrest = 0;
        last_fully_paid_installment_no = 0;
        //std::cerr<<"One Time Division\n";

        for (int i = this->loan_list[loan_id]->getFirstInstallmentNumber(); i<this->loan_list[loan_id]->getLastInstallmentNumber(); i++) {
            if (total_transaction_amount > total_installment) {
                total_principle += this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                total_interrest += this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                total_installment = total_principle + total_interrest;
                last_fully_paid_installment_no = i;
            }
            if (total_transaction_amount == total_installment && total_transaction_amount > 0) {
                last_fully_paid_installment_no = i;
                break;
            } else if (total_transaction_amount < total_installment) {
                last_fully_paid_installment_no = (i - 1 > 0 ? i - 1 : 0);

                total_principle -= this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                total_interrest -= this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                total_installment = total_principle + total_interrest;

                //total_principle+=this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                if (total_installment + this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount <= total_transaction_amount) {
                    total_interrest += this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                    total_installment = total_principle + total_interrest;
                }
                if (total_installment + this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount <= total_transaction_amount) {
                    total_principle += this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                    total_installment = total_principle + total_interrest;
                } else {
                    total_principle += total_transaction_amount - total_installment;
                    total_installment = total_principle + total_interrest;
                }

                break;
            }
            //std::cerr<<"Divide return "<<total_installment<<" "<<total_principle<<" "<<total_interrest<<" "<<last_fully_paid_installment_no<<"\n";
        }

    } else if (this->loan_list[loan_id]->service_charge_taken_time == "EFS" || this->loan_list[loan_id]->service_charge_taken_time == "EFRAP" || this->loan_list[loan_id]->GetDecline_Period() == "DB") {
        total_installment = 0;
        total_principle = 0;
        total_interrest = 0;
        last_fully_paid_installment_no = 0;
        //std::cerr<<"One Time Division\n";

        for (int i = this->loan_list[loan_id]->getFirstInstallmentNumber(); i<this->loan_list[loan_id]->getLastInstallmentNumber(); i++) {

            if (total_transaction_amount > total_installment) {
                if (this->loan_list[loan_id]->getLastInstallmentNumber() == 1 && i == 0) {
                    if (total_transaction_amount < this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount) {
                        total_principle += total_transaction_amount;
                        total_interrest += 0;
                    } else {
                        total_principle += this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                        total_interrest += this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                    }
                    total_installment = total_principle + total_interrest;
                } else {
                    total_principle += this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                    total_interrest += this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                    total_installment = total_principle + total_interrest;
                    last_fully_paid_installment_no = i;

                }



            }
            if (total_transaction_amount == total_installment && total_transaction_amount > 0) {
                last_fully_paid_installment_no = i;
                break;
            } else if (total_transaction_amount < total_installment) {
                last_fully_paid_installment_no = (i - 1 > 0 ? i - 1 : 0);

                total_principle -= this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                total_interrest -= this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                total_installment = total_principle + total_interrest;

                //total_principle+=this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                if (total_installment + this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount <= total_transaction_amount) {
                    total_interrest += this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                    total_installment = total_principle + total_interrest;
                } else if (this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount > total_transaction_amount) {
                    //std::cerr << this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount << " " << total_transaction_amount << " HI\n";
                    total_interrest += total_transaction_amount;
                    if (this->loan_list[loan_id]->GetDecline_Period() == "DB") {
                        total_transaction_amount = 0;
                    }
                } else if (total_installment + this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount > total_transaction_amount && this->loan_list[loan_id]->GetDecline_Period() == "DB") {
                    total_interrest += total_transaction_amount - total_installment;
                }

                if (total_installment + this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount <= total_transaction_amount) {
                    total_principle += this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                    total_installment = total_principle + total_interrest;

                } else if (total_installment + this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount > total_transaction_amount && this->loan_list[loan_id]->GetDecline_Period() == "DB") {
                    if (total_transaction_amount - total_interrest > 0) {
                        total_principle = total_transaction_amount - total_interrest;
                    } else {
                        total_principle = 0;
                    }
                    total_installment = total_principle + total_interrest;
                } else {
                    total_principle += total_transaction_amount - total_installment;
                    total_installment = total_principle + total_interrest;

                }
                
                break;
            }
        }

    } else if (this->loan_list[loan_id]->service_charge_taken_time == "ASM") {
        total_installment = 0;
        total_principle = 0;
        total_interrest = 0;
        last_fully_paid_installment_no = 0;
        //std::cerr<<"One Time Division\n";

        for (int i = this->loan_list[loan_id]->getFirstInstallmentNumber(); i<this->loan_list[loan_id]->getLastInstallmentNumber(); i++) {
            if (total_transaction_amount > total_installment) {
                //total_principle+= this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                //total_interrest+= this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                // Updated by Murad                 
                if ((total_transaction_amount - total_installment) >= (this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount + this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount)) {
                    total_principle += this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                    total_interrest += this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                } else if ((total_transaction_amount - total_installment) >= this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount && (total_transaction_amount - total_installment) < (this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount + this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount)) {
                    total_principle += this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                    total_interrest += (total_transaction_amount - total_installment) - this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                } else {
                    total_principle += total_transaction_amount - total_installment;
                    total_interrest += 0;
                }
                total_installment = total_principle + total_interrest;
                last_fully_paid_installment_no = i;
            }
            if (total_transaction_amount == total_installment && total_transaction_amount > 0) {
                last_fully_paid_installment_no = i;
                break;
            } else if (total_transaction_amount < total_installment) {
                last_fully_paid_installment_no = (i - 1 > 0 ? i - 1 : 0);

                total_principle -= this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                total_interrest -= this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                total_installment = total_principle + total_interrest;

                //total_principle+=this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                if (total_installment + this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount <= total_transaction_amount) {
                    total_principle += this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                    total_installment = total_principle + total_interrest;
                }
                if (total_installment + this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount <= total_transaction_amount) {
                    total_interrest += this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                    total_installment = total_principle + total_interrest;
                } else {
                    total_interrest += total_transaction_amount - total_installment;
                    total_installment = total_principle + total_interrest;
                }


                break;
            }
            //std::cerr<<"Divide return "<<total_installment<<" "<<total_principle<<" "<<total_interrest<<" "<<last_fully_paid_installment_no<<"\n";
        }

    } else {
        total_installment = 0;
        total_principle = 0;
        total_interrest = 0;
        last_fully_paid_installment_no = 0;

        //std::cerr<<total_installment<<std::endl;
        //std::cerr<<this->loan_list[loan_id]->GetRepayment_frequency()<<std::endl;

        if (total_transaction_amount >= this->loan_list[loan_id]->GetTotal_payable_amount()) {
            total_principle = this->loan_list[loan_id]->GetLoan_amount();
            total_interrest = total_transaction_amount - total_principle;
            total_installment = total_principle + total_interrest;
            last_fully_paid_installment_no = this->loan_list[loan_id]->GetActualNumber_of_installment();

        } else {
            if (this->loan_list[loan_id]->GetInterest_calculation_method().compare("F") == 0) {
                
                if(this->loan_list[loan_id]->service_charge_taken_time=="SCTBI"){
                    double total_payable_amount=this->loan_list[loan_id]->GetTotal_payable_amount();
                    double interest_amount=total_payable_amount-this->loan_list[loan_id]->GetLoan_amount();
                    //std::cerr<< interest_amount<<" Total interest Amount "<< total_payable_amount << std::endl;
                    if(total_transaction_amount<=interest_amount){
                        total_interrest=total_transaction_amount;
                        total_principle=0;
                        total_installment=total_transaction_amount;
                    }else{
                        total_interrest=interest_amount;
                        total_principle=total_transaction_amount-interest_amount;
                        total_installment=total_transaction_amount;
                    }
                }else{
                    total_principle = total_transaction_amount / this->loan_list[loan_id]->interest_index;
                    //total_principle=_round(total_principle);
                    total_interrest = total_transaction_amount - total_principle;
                    //total_interrest=_round(total_interrest);
                    total_installment = total_principle + total_interrest;
                }
                //total_installment=_round(total_installment);
            } else if (this->loan_list[loan_id]->GetInterest_calculation_method().compare("DFD") == 0) {
                    total_principle = (total_transaction_amount) / this->loan_list[loan_id]->interest_index;
                    total_interrest = total_transaction_amount - total_principle;
                    total_installment = total_principle + total_interrest;
                    if(this->loan_list[loan_id]->repayment_mode_for_monthly_loan=="LISC"){
                        double interest_amount1=this->loan_list[loan_id]->schedule_datum[0].interrest_installment_amount;                       
                        double temp_total_transaction_amount=total_transaction_amount-interest_amount1;
                        if(temp_total_transaction_amount<0){
                            total_principle=0;
                            total_interrest=total_transaction_amount;
                        }else{
                            double new_interest_index=(this->loan_list[loan_id]->GetTotal_payable_amount()-interest_amount1)/this->loan_list[loan_id]->GetLoan_amount();
                            total_principle = (temp_total_transaction_amount) / new_interest_index;
                            total_interrest = temp_total_transaction_amount - total_principle+interest_amount1;
                            //std::cerr<< "temp_total_transaction_amount "<<temp_total_transaction_amount<<" total_principle "<<total_principle<<" Interest Amount "<< interest_amount1 <<" total_interrest "<<total_interrest<<std::endl;
                            total_installment = total_principle + total_interrest;
                        }
                    }
            } else {
                double P = this->loan_list[loan_id]->GetLoan_amount();
                double X = this->loan_list[loan_id]->schedule_datum[0].installment_amount;
                double n = total_transaction_amount / X;
                double r = this->loan_list[loan_id]->GetInterest_rate() / ((this->loan_list[loan_id]->GetActualNumber_of_installment()*12) / this->loan_list[loan_id]->GetLoan_period_in_month()*100);
                if (this->loan_list[loan_id]->GetMode_of_interest().compare("T") == 0)
                    r = this->loan_list[loan_id]->GetInterest_rate() / ((this->loan_list[loan_id]->GetActualNumber_of_installment()*12) / this->loan_list[loan_id]->GetLoan_period_in_month()*1000);

                total_installment = total_transaction_amount;
                total_interrest += ((((P * r - X)*(pow(r + 1, n) - 1)) / r) + n * X) - total_interrest;
                //total_interrest=_round(total_interrest);
                total_principle = total_installment - total_interrest;
                //total_principle=_round(total_principle);

                //Added for Negative Principle by MURAD
                if (this->loan_list[loan_id]->GetLoan_amount() < total_principle) {
                    total_interrest = total_interrest + (total_principle - this->loan_list[loan_id]->GetLoan_amount());
                    total_principle = total_principle - (total_principle - this->loan_list[loan_id]->GetLoan_amount());
                }              
                double db_interest_amount = this->loan_list[loan_id]->GetTotal_payable_amount() - this->loan_list[loan_id]->GetLoan_amount();
                if (total_interrest > db_interest_amount) {
                    if (total_principle<this->loan_list[loan_id]->GetLoan_amount()) {
                        total_principle += total_interrest - db_interest_amount;
                    }
                    total_interrest = db_interest_amount;
                    if(total_principle>this->loan_list[loan_id]->GetLoan_amount()){
                        total_interrest+=total_principle-this->loan_list[loan_id]->GetLoan_amount();
                        total_principle=this->loan_list[loan_id]->GetLoan_amount();
                    }                   
                }
            }
        }

    }

    if (this->loan_list[loan_id]->GetInterest_calculation_method().compare("DFD") != 0) {
        total_installment = _round(total_installment);
        total_principle = _round(total_principle);
        total_interrest = _round(total_interrest);
    } else {
        total_installment = _round10(total_installment);
        total_principle = _round10(total_principle);
        total_interrest = _round10(total_interrest);
    }
    if(this->loan_list[loan_id]->GetLoan_amount()<total_principle){
        total_interrest+=total_principle-this->loan_list[loan_id]->GetLoan_amount();
        total_principle=this->loan_list[loan_id]->GetLoan_amount();
    }
    std::vector<double> installment_principle_interest_installment_no;
    installment_principle_interest_installment_no.push_back(total_installment);
    installment_principle_interest_installment_no.push_back(total_principle);
    installment_principle_interest_installment_no.push_back(total_interrest);
    installment_principle_interest_installment_no.push_back(last_fully_paid_installment_no);
    return installment_principle_interest_installment_no;
}

zval Loan_scheduler::get_loan_installment_information_by_loan_id_trasaction_amount_previous_transaction_amount(int loan_id, double current_transaction_amount, double previous_total_transaction_amount, double previous_total_principle_amount = 0, double previous_total_interest_amount = 0) {

    //std::cerr<<"Loan ID"<<loan_id<<" Loan Type "<<this->loan_list[loan_id]->GetLoan_type()<<std::endl;
    zval array1;
//    MAKE_STD_ZVAL(array1);
    array_init(&array1);

    double pre_total_installment = 0;
    double pre_total_principle = 0;
    double pre_total_interrest = 0;
    int pre_last_installment_no = 0;

    double cur_total_installment = 0;
    double cur_total_principle = 0;
    double cur_total_interrest = 0;
    int cur_last_installment_no = 0;

    double total_installment = 0;
    double total_principle = 0;
    double total_interrest = 0;
    if (current_transaction_amount == 0) {
        add_assoc_double(&array1, "installment_amount", 0);
        add_assoc_double(&array1, "principle_installment_amount", 0);
        add_assoc_double(&array1, "interrest_installment_amount", 0);

        for (int i = this->loan_list[loan_id]->getFirstInstallmentNumber(); i<this->loan_list[loan_id]->getLastInstallmentNumber(); i++) {
            if (previous_total_transaction_amount == 0) {
                break;
            }
            previous_total_transaction_amount -= this->loan_list[loan_id]->schedule_datum[i].installment_amount;
            //std::cerr<<previous_total_transaction_amount;
            //std::cerr<<"...MURAD222....\n";
            if (previous_total_transaction_amount >= 0) {
                cur_last_installment_no = i + 1;
            }
        }
        add_assoc_long(&array1, "last_fully_paid_installment_number", cur_last_installment_no);

        return array1;
    }

    if (this->loan_list[loan_id]->GetLoan_type().compare("O") == 0) {

        double total_transaction_amount = current_transaction_amount + previous_total_transaction_amount;
        total_installment = 0;
        total_principle = 0;
        total_interrest = 0;
        //std::cerr<<"One Time Division\n";

        double one_time_current_total_installment = 0;
        double one_time_current_total_principle = 0;
        double one_time_current_total_interrest = 0;
        int one_time_current_cur_last_installment_no;

        double one_time_previous_total_installment = 0;
        double one_time_previous_total_principle = 0;
        double one_time_previous_total_interrest = 0;
        int one_time_previous_cur_last_installment_no;

        for (int i = this->loan_list[loan_id]->getFirstInstallmentNumber(); i<this->loan_list[loan_id]->getLastInstallmentNumber(); i++) {

            if (total_transaction_amount < one_time_current_total_installment) {
                one_time_current_total_installment += this->loan_list[loan_id]->schedule_datum[i].installment_amount;
                one_time_current_total_principle += this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                one_time_current_total_interrest += this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                one_time_current_cur_last_installment_no = i + 1;
            }

            if (previous_total_transaction_amount < one_time_previous_total_installment) {
                one_time_previous_total_installment += this->loan_list[loan_id]->schedule_datum[i].installment_amount;
                one_time_previous_total_principle += this->loan_list[loan_id]->schedule_datum[i].principle_installment_amount;
                one_time_previous_total_interrest += this->loan_list[loan_id]->schedule_datum[i].interrest_installment_amount;
                one_time_previous_cur_last_installment_no = i + 1;
            }
        }


        total_installment = one_time_current_total_installment;
        total_principle = one_time_current_total_principle;
        total_interrest = one_time_current_total_interrest;
        cur_last_installment_no = one_time_current_cur_last_installment_no;
        if (total_transaction_amount > total_installment) {
            total_interrest += total_transaction_amount - total_installment;
            total_installment = total_principle + total_interrest;
        }


        total_installment -= previous_total_transaction_amount;
        total_principle -= previous_total_principle_amount;
        total_interrest -= previous_total_interest_amount;

        add_assoc_double(&array1, "installment_amount", total_installment);
        add_assoc_double(&array1, "principle_installment_amount", total_principle);
        add_assoc_double(&array1, "interrest_installment_amount", total_interrest);
        add_assoc_long(&array1, "last_fully_paid_installment_number", cur_last_installment_no);

        return array1;
    }

    std::vector<double> tmp = this->divide_transaction_to_installment_principle_interest_installment_no(loan_id, previous_total_transaction_amount);
    //std::cerr<<"After return from divide first";
    pre_total_installment = tmp[0];
    pre_total_principle = tmp[1];
    pre_total_interrest = tmp[2];
    pre_last_installment_no = tmp[3];

    tmp = this->divide_transaction_to_installment_principle_interest_installment_no(loan_id, current_transaction_amount + previous_total_transaction_amount);
    //std::cerr<<"After return from divide second";
    cur_total_installment = tmp[0];
    cur_total_principle = tmp[1];
    cur_total_interrest = tmp[2];
    cur_last_installment_no = tmp[3];

    //std::cerr<<"\npre_total_installment "<<pre_total_installment<<" pre_total_principle "<<pre_total_principle<<" pre_total_interrest "<<pre_total_interrest<<std::endl;
    //std::cerr<<"previous_total_transaction_amount "<<previous_total_transaction_amount<<" previous_total_principle_amount "<<previous_total_principle_amount<<" previous_total_interest_amount "<<previous_total_interest_amount<<std::endl;
    //std::cerr<<"cur_total_installment "<<cur_total_installment<<" cur_total_principle "<<cur_total_principle<<" cur_total_interrest "<<cur_total_interrest<<std::endl;

    if (this->loan_list[loan_id]->GetTotal_payable_amount() == current_transaction_amount + previous_total_transaction_amount) {
        cur_total_principle = this->loan_list[loan_id]->GetLoan_amount();
        cur_total_interrest = this->loan_list[loan_id]->GetTotal_payable_amount() - this->loan_list[loan_id]->GetLoan_amount();
        cur_last_installment_no = this->loan_list[loan_id]->GetActualNumber_of_installment();
    } else if (this->loan_list[loan_id]->GetTotal_payable_amount() < current_transaction_amount + previous_total_transaction_amount) {
        cur_total_principle = this->loan_list[loan_id]->GetLoan_amount();
        //cur_total_interrest=this->loan_list[loan_id]->GetTotal_payable_amount()-this->loan_list[loan_id]->GetLoan_amount();
        cur_last_installment_no = this->loan_list[loan_id]->GetActualNumber_of_installment();
    }
    if (previous_total_principle_amount != 0 && previous_total_principle_amount != pre_total_principle)
        pre_total_principle = previous_total_principle_amount;
    if (previous_total_interest_amount != 0 && previous_total_interest_amount != pre_total_interrest)
        pre_total_interrest = previous_total_interest_amount;

    if (current_transaction_amount == 0) {
        add_assoc_double(&array1, "installment_amount", 0);
        add_assoc_double(&array1, "principle_installment_amount", 0);
        add_assoc_double(&array1, "interrest_installment_amount", 0);
        add_assoc_long(&array1, "last_fully_paid_installment_number", pre_last_installment_no);

        return array1;
    }
    total_installment = cur_total_installment - pre_total_installment;
    total_principle = cur_total_principle - pre_total_principle;
    total_interrest = cur_total_interrest - pre_total_interrest;

    /*if (total_interrest < 0 && this->loan_list[loan_id]->GetDecline_Period() == "DB") {
        total_principle = total_installment;
        total_interrest = 0;
    }*/

    //std::cerr<<"total_installment "<<total_installment<<" total_principle "<<total_principle<<" total_interrest "<<total_interrest<<std::endl;

    if (this->loan_list[loan_id]->GetInterest_calculation_method().compare("DFD") != 0) {
        total_installment = _round(total_installment);
        total_principle = _round(total_principle);
        total_interrest = _round(total_interrest);
    } else {
        total_installment = _round10(total_installment);
        total_principle = _round10(total_principle);
        total_interrest = _round10(total_interrest);
    }

    add_assoc_double(&array1, "installment_amount", total_installment);
    add_assoc_double(&array1, "principle_installment_amount", total_principle);
    add_assoc_double(&array1, "interrest_installment_amount", total_interrest);
    add_assoc_long(&array1, "last_fully_paid_installment_number", cur_last_installment_no);

    return array1;
}

