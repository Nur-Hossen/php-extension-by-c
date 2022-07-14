#include "php_schedule.h"
#include "Loan_scheduler.h"
#include <vector>
#include <boost/format.hpp>
#include <sys/ioctl.h>
#include <net/if.h> 
#include <unistd.h>
#include <netinet/in.h>

zend_object_handlers Loan_scheduler_object_handlers;
zend_class_entry *sch_ce;

typedef struct _loan_scheduler_object {
    Loan_scheduler *sch;
    zend_object std;
} Loan_scheduler_object;

static inline Loan_scheduler_object * php_custom_object_fetch_object(zend_object *obj) {
    return (Loan_scheduler_object *) ((char *) obj - XtOffsetOf(Loan_scheduler_object, std));
}

void sch_free_storage(void *object TSRMLS_DC) {
    Loan_scheduler_object *obj = (Loan_scheduler_object *) object;
    delete obj->sch;

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
    //std::cerr<<"Object Deleted\n";
}

/* PHP 7 */
static void sch_destroy_object(zend_object *object) {
    Loan_scheduler_object *my_obj;
    my_obj = (Loan_scheduler_object *) ((char *) object - XtOffsetOf(Loan_scheduler_object, std));

    /* Now we could do something with my_obj->my_custom_buffer, like sending it
       on a socket, or flush it to a file, or whatever, but not free it here */
    my_obj->sch = NULL;
    zend_objects_destroy_object(object); /* call __destruct() from userland */
}

/* PHP 7 */
static void sch_free_object(zend_object *object) {
    Loan_scheduler_object *my_obj;

    my_obj = (Loan_scheduler_object *) ((char *) object - XtOffsetOf(Loan_scheduler_object, std));

    efree(my_obj->sch); /* Free your custom resources */
    delete my_obj->sch;
    zend_object_std_dtor(object); /* call Zend's free handler, which will free object properties */
}

zend_object *sch_create_handler(zend_class_entry *type TSRMLS_DC) {
    Loan_scheduler_object *obj = (Loan_scheduler_object *) ecalloc(1, sizeof (*obj) + zend_object_properties_size(type));
    zend_object_std_init(&obj->std, type);
    object_properties_init((zend_object*) &(obj->std), type);
    obj->std.handlers = &Loan_scheduler_object_handlers;
    return &obj->std;
}

PHP_METHOD(Loan_scheduler, __construct) {
    
    //std::cerr<<"CC In Construct"<<std::endl;
    Loan_scheduler *sch = NULL;

    sch = new Loan_scheduler();
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    obj->sch = sch;
}

PHP_METHOD(Loan_scheduler, initialize) {
    
//    std::cerr<<"CC In Initialize"<<std::endl; exit(1);
    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    char *sys_date;
    size_t sys_date_len;
    zend_long b_id;
    zend_long s_id;
    char *d_frm;
    size_t d_frm_len;
    char *d_to;
    size_t d_to_len;
    
    zend_long org_flag=0;

    zval *data1;
    HashTable *arr_hash1;

    zval *data2;
    HashTable *arr_hash2;

    zval *data3;
    HashTable *arr_hash3;

    zval *data4;
    HashTable *arr_hash4;

    zval *data5;
    HashTable *arr_hash5;

//    std::cerr<<"CC In Parameter parsing"<<std::endl;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sllsshhhhh|l", &sys_date, &sys_date_len,&b_id, &s_id, &d_frm, &d_frm_len, &d_to, &d_to_len,  &arr_hash1, &arr_hash2, &arr_hash3, &arr_hash4, &arr_hash5, &org_flag) == FAILURE) {
        return;
    }
    boost::gregorian::date *system_date = new boost::gregorian::date(boost::gregorian::from_simple_string(sys_date));
//    std::cerr<<*system_date<<std::endl;
//    exit(1);

    //typedef __gnu_cxx::hash_map<std::string, boost::gregorian::date> br_hash;
    
//    std::cerr<<"CC In branch_holiday"<<std::endl; 
    typedef __gnu_cxx::hash_map<boost::gregorian::date, boost::gregorian::date,boost::hash<boost::gregorian::date> > br_hash;
    
    br_hash b_holiday;
    br_hash *b_holiday_s = new br_hash();
//    ulong key_tets;
//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {
    ZEND_HASH_FOREACH_VAL(arr_hash1, data1){  
        if (Z_TYPE_P(data1) == IS_STRING) {
//            b_holiday[std::string(Z_STRVAL_P(data1))] = boost::gregorian::from_simple_string(Z_STRVAL_P(data1));
           b_holiday[boost::gregorian::from_simple_string(Z_STRVAL_P(data1))] = boost::gregorian::from_simple_string(Z_STRVAL_P(data1));
            (*b_holiday_s)[boost::gregorian::from_simple_string(Z_STRVAL_P(data1))] = boost::gregorian::from_simple_string(Z_STRVAL_P(data1));

        }
    } ZEND_HASH_FOREACH_END();


//    std::cerr<<"CC In samity_holiday"<<std::endl; 
    //typedef __gnu_cxx::hash_map<std::string, boost::gregorian::date> br_hash;
    typedef __gnu_cxx::hash_map<int, br_hash> samity_date_hash;
    //typedef std::map<int, br_hash> samity_date_hash;
    samity_date_hash samity_holiday_list;
    typedef __gnu_cxx::hash_map<int, br_hash*> samity_date_hash_s;
    samity_date_hash_s *samity_holiday_list_s=new samity_date_hash_s();


//    for (zend_hash_internal_pointer_reset_ex(arr_hash2, &pointer2); zend_hash_get_current_data_ex(arr_hash2, (void**) &data2, &pointer2) == SUCCESS; zend_hash_move_forward_ex(arr_hash2, &pointer2)
//            ) {
//
        zend_string *key;
//        uint key_len;
        ulong index;

    ZEND_HASH_FOREACH_KEY_VAL(arr_hash2, index, key, data2) {
            //if (index) {

            br_hash s_holiday;
            br_hash *s_holiday_s = new br_hash();
            zval temp;

            zval *data21;
            HashTable *arr_hash21;

            temp = *data2;
            zval_copy_ctor(&temp);
            convert_to_array(&temp);

            arr_hash21 = Z_ARRVAL_P(&temp);

             ZEND_HASH_FOREACH_VAL(arr_hash21, data21) {
                //for (zend_hash_internal_pointer_reset_ex(arr_hash21, &pointer21); zend_hash_get_current_data_ex(arr_hash21, (void**) &data21, &pointer21) == SUCCESS; zend_hash_move_forward_ex(arr_hash21, &pointer21)) {

                if (Z_TYPE_P(data21) == IS_STRING) {
                    std::string temp_date(Z_STRVAL_P(data21));
                    if (!temp_date.empty()) {

                        boost::gregorian::date date_fro = boost::gregorian::from_simple_string(temp_date);
                    }
                    //s_holiday[std::string(Z_STRVAL_P(data21))] = (boost::gregorian::from_simple_string(std::string(Z_STRVAL_P(data21))));
                    s_holiday[(boost::gregorian::from_simple_string(std::string(Z_STRVAL_P(data21))))] = (boost::gregorian::from_simple_string(std::string(Z_STRVAL_P(data21))));
                    (*s_holiday_s)[(boost::gregorian::from_simple_string(std::string(Z_STRVAL_P(data21))))] = (boost::gregorian::from_simple_string(std::string(Z_STRVAL_P(data21))));
                    //std::cerr<<"I'm here "<<temp_date<<std::endl;
                }

            }


            ZEND_HASH_FOREACH_END();

            samity_holiday_list[index] = s_holiday;
            (*samity_holiday_list_s)[index] = s_holiday_s;
            //}

        }
        ZEND_HASH_FOREACH_END();

//    std::cerr<<"CC In loans"<<std::endl;
    //std::vector<loan> uu_loans;
    typedef __gnu_cxx::hash_map<int, loan*> loan_hash;
    loan_hash loan_list;

//    for (zend_hash_internal_pointer_reset_ex(arr_hash3, &pointer3); zend_hash_get_current_data_ex(arr_hash3, (void**) &data3, &pointer3) == SUCCESS; zend_hash_move_forward_ex(arr_hash3, &pointer3)) {
    ZEND_HASH_FOREACH_VAL(arr_hash3, data3){    
        zval temp;

        zval *data31;
        HashTable *arr_hash31;

        temp = *data3;
        zval_copy_ctor(&temp);
        convert_to_array(&temp);

        arr_hash31 = Z_ARRVAL_P(&temp);
//        array_count31 = zend_hash_num_elements(arr_hash31);

        //std::cerr<<"CC In First Loop"<<std::endl;

        //loan ll;
        int ID=0, branch_id, samity_id, member_id, product_id,purpose_id, number_of_installment, current_status, is_loan_fully_paid, is_authorized;
        boost::gregorian::date disburse_date, first_repayment_date, last_repayment_date, loan_fully_paid_date,loan_closing_date;
        std::string repayment_frequency,grace_period, interest_calculation_method, mode_of_interest,decline_period;
        double interest_rate, loan_amount, loan_period_in_month,extra_installment_amount,installment_amountdb,loan_period_in_day ;
        std::string transfer_in_date,transfer_out_date;
        int is_transferred_in;
        int is_transferred_out;
        
        int enforce_number_of_installment=0;
        int is_show_rec_after_loan_period=0;
        int is_loan_period_in_day_fixed=0;
        int enforced_install_number=0;
        int cycle=1;
        int original_loan_id=0;
        int skip_last_week_calculation_monthly=0;
        std::string loan_type;
        
        double penalty=0;
        double waiver=0;
        double rebate=0;
        
        boost::gregorian::date rebate_date;
        
        std::string interest_payment_frequency="NA";
        std::string repayment_mode_for_monthly_loan="NA";
        std::string repayment_day_or_week="NA";
        std::string service_charge_taken_time="NA";
        
        double interest_index=1;
        
        last_repayment_date=boost::gregorian::from_simple_string("1900-01-01");
        
        __gnu_cxx::hash_map<std::string, std::string> loan_transfer_daychange__info;
        __gnu_cxx::hash_map<std::string, int> loan_transfer_samitychange__info;
        __gnu_cxx::hash_map<std::string, double> one_time_weekly_loans_transactions__info;

//        for (zend_hash_internal_pointer_reset_ex(arr_hash31, &pointer31); zend_hash_get_current_data_ex(arr_hash31, (void**) &data31, &pointer31) == SUCCESS; zend_hash_move_forward_ex(arr_hash31, &pointer31)) {
        zend_string *key;
//      uint key_len;
        ulong index;
        ZEND_HASH_FOREACH_KEY_VAL(arr_hash31, index, key, data31){        
            //std::cerr<<"CC In Second Loop"<<std::endl;
            
            zval temp;
//            if (zend_hash_get_current_key_ex(arr_hash31, &key, &key_len, &index, 0, &pointer31) != HASH_KEY_NON_EXISTANT) {
            if(key){    
                //std::cerr<< key <<std::endl;
                if (strcmp(key->val, "id") == 0) {
                    //std::cerr<<"ID ";
                    std::string temp1(Z_STRVAL_P(data31));
                    ID = atoi(temp1.c_str());
                } else if (strcmp(key->val, "branch_id") == 0) {
                     //std::cerr<<"branch_id ";
                    std::string temp1(Z_STRVAL_P(data31));
                    branch_id = atoi(temp1.c_str());
                    //ll.SetBranch_id(branch_id);
                }
                else if (strcmp(key->val, "samity_id") == 0) {
                    //std::cerr<<"samity_id ";
                    std::string temp1(Z_STRVAL_P(data31));
                    samity_id = atoi(temp1.c_str());
                } else if (strcmp(key->val, "member_id") == 0) {
                    //std::cerr<<"member_id ";
                    std::string temp1(Z_STRVAL_P(data31));
                    member_id = atoi(temp1.c_str());
                } else if (strcmp(key->val, "product_id") == 0) {
                     //std::cerr<<"product_id ";
                    std::string temp1(Z_STRVAL_P(data31));
                    product_id = atoi(temp1.c_str());
                } else if (strcmp(key->val, "purpose_id") == 0) {
                     //std::cerr<<"purpose_id ";
                    std::string temp1(Z_STRVAL_P(data31));
                    purpose_id = atoi(temp1.c_str());
                }  
                else if (strcmp(key->val, "number_of_installment") == 0) {
                    //std::cerr<<"number_of_installment ";
                    std::string temp1(Z_STRVAL_P(data31));
                    number_of_installment = atoi(temp1.c_str());
                }
                else if (strcmp(key->val, "disburse_date") == 0) {
                    //std::cerr<<"disburse_date ";
                    disburse_date = boost::gregorian::from_simple_string(Z_STRVAL_P(data31));
                }
                else if (strcmp(key->val, "first_repayment_date") == 0) {
                    //std::cerr<<"first_repayment_date ";
                    first_repayment_date = boost::gregorian::from_simple_string(Z_STRVAL_P(data31));

                    //std::cerr<<"first_repayment_date "<<first_repayment_date<<std::endl;
                    //ll.SetFirst_repayment_date(first_repayment_date);
                } else if (strcmp(key->val, "last_repayment_date") == 0) {
                    //std::cerr<<"last_repayment_date ";
                    last_repayment_date = boost::gregorian::from_simple_string(Z_STRVAL_P(data31));

                    //std::cerr<<"last_repayment_date "<<last_repayment_date<<std::endl;
                    //ll.SetLast_repayment_date(last_repayment_date);
                } else if (strcmp(key->val, "repayment_frequency") == 0) {
                    //std::cerr<<"repayment_frequency ";
                    repayment_frequency = std::string(Z_STRVAL_P(data31));

                    //std::cerr<<"repayment_frequency "<<repayment_frequency<<std::endl;
                    //ll.SetRepayment_frequency(repayment_frequency);
                }else if (strcmp(key->val, "grace_period") == 0) {
                    //std::cerr<<"repayment_frequency ";
                    grace_period = std::string(Z_STRVAL_P(data31));

                    //std::cerr<<"repayment_frequency "<<repayment_frequency<<std::endl;
                    //ll.SetRepayment_frequency(repayment_frequency);
                }
                else if (strcmp(key->val, "loan_period_in_month") == 0) {
                    //std::cerr<<"loan_period_in_month ";
                    std::string temp1(Z_STRVAL_P(data31));
                    loan_period_in_month = strtod(temp1.c_str(), NULL);
                }
                else if (strcmp(key->val, "loan_period_in_day") == 0) {
                    //std::cerr<<"loan_period_in_month ";
                    std::string temp1(Z_STRVAL_P(data31));
                    loan_period_in_day = strtod(temp1.c_str(), NULL);
                }
                else if (strcmp(key->val, "is_loan_period_in_day_fixed") == 0) {                  
                    std::string temp1(Z_STRVAL_P(data31));
                    is_loan_period_in_day_fixed = strtod(temp1.c_str(), NULL);
                }
                else if (strcmp(key->val, "interest_rate") == 0) {
                     //std::cerr<<"interest_rate ";
                    std::string temp1(Z_STRVAL_P(data31));
                    interest_rate = strtod(temp1.c_str(), NULL);
                } else if (strcmp(key->val, "loan_amount") == 0) {
                    //std::cerr<<"loan_amount ";
                    std::string temp1(Z_STRVAL_P(data31));
                    loan_amount = strtod(temp1.c_str(), NULL);
                } else if (strcmp(key->val, "current_status") == 0) {
                     //std::cerr<<"current_status ";
                    std::string temp1(Z_STRVAL_P(data31));
                    current_status = atoi(temp1.c_str());
                }
                else if (strcmp(key->val, "loan_fully_paid_date") == 0) {

                    //std::cerr<<"loan_fully_paid_date ";
                    if(Z_TYPE_P(data31) == IS_STRING)
                    loan_fully_paid_date = boost::gregorian::from_simple_string(Z_STRVAL_P(data31));
                } else if (strcmp(key->val, "is_loan_fully_paid") == 0) {
                    //std::cerr<<"is_loan_fully_paid ";
                    std::string temp1(Z_STRVAL_P(data31));
                    is_loan_fully_paid = atoi(temp1.c_str());
                }
                else if (strcmp(key->val, "is_authorized") == 0) {
                    //std::cerr<<"is_authorized ";
                    std::string temp1(Z_STRVAL_P(data31));
                    is_authorized = atoi(temp1.c_str());
                }
                else if (strcmp(key->val, "skip_last_week_calculation_monthly") == 0) {
                    //std::cerr<<"is_authorized ";
                    std::string temp1(Z_STRVAL_P(data31));
                    skip_last_week_calculation_monthly = atoi(temp1.c_str());
                }
                else if (strcmp(key->val, "interest_calculation_method") == 0) {
                    //std::cerr<<"interest_calculation_method ";
                    interest_calculation_method = std::string(Z_STRVAL_P(data31));
                }
                else if (strcmp(key->val, "decline_period") == 0) {                 
                    decline_period = std::string(Z_STRVAL_P(data31));
                }
                else if (strcmp(key->val, "mode_of_interest") == 0) {
                     //std::cerr<<"mode_of_interest ";
                    mode_of_interest = std::string(Z_STRVAL_P(data31));
                }
                else if (strcmp(key->val, "loan_type") == 0) {
                    //std::cerr<<"loan_type ";
                    loan_type = std::string(Z_STRVAL_P(data31));

                    //std::cerr<<"loan_type "<<loan_type<<std::endl;
                    //ll.SetRepayment_frequency(repayment_frequency);
                }
                else if (strcmp(key->val, "loan_closing_date") == 0) {
                    //std::cerr<<"loan_closing_date ";
                    if(Z_TYPE_P(data31) == IS_STRING)
                    loan_closing_date = boost::gregorian::from_simple_string(Z_STRVAL_P(data31));

                    //std::cerr<<"loan_closing_date "<<loan_closing_date<<std::endl;
                    //ll.SetRepayment_frequency(repayment_frequency);
                }
                else if (strcmp(key->val, "extra_installment_amount") == 0) {
                    //std::cerr<<"extra_installment_amount ";
                    std::string temp1(Z_STRVAL_P(data31));
                    extra_installment_amount = strtod(temp1.c_str(), NULL);
                }
                else if (strcmp(key->val, "transfer_in_date") == 0) {
                    //std::cerr<<"transfer_in_date ";
                    is_transferred_in=0;
                    if(Z_TYPE_P(data31) == IS_STRING){
                        is_transferred_in=1;
                        transfer_in_date.assign(Z_STRVAL_P(data31));
                    }
                }
                else if (strcmp(key->val, "transfer_out_date") == 0) {
                    //std::cerr<<"transfer_out_date ";
                    is_transferred_out=0;
                    if(Z_TYPE_P(data31) == IS_STRING){
                        is_transferred_out=1;
                        transfer_out_date.assign(Z_STRVAL_P(data31));
                    }
                }
                else if (strcmp(key->val, "original_loan_id") == 0) {
                    //std::cerr<<"original_loan_id ";
                    std::string temp1(Z_STRVAL_P(data31));
                    original_loan_id = atoi(temp1.c_str());
                }
                else if (strcmp(key->val, "transfers") == 0) {

                    //std::cerr<<"transfers\n";
                    zval temp3;

                    zval *data32;
                    HashTable *arr_hash32;

                    temp3 = *data31;
                    zval_copy_ctor(&temp3);
                    convert_to_array(&temp3);

                    arr_hash32 = Z_ARRVAL_P(&temp3);
//                    array_count32 = zend_hash_num_elements(arr_hash32);
                    
                    std::string date;
                    std::string day;

//                    for (zend_hash_internal_pointer_reset_ex(arr_hash32, &pointer32); zend_hash_get_current_data_ex(arr_hash32, (void**) &data32, &pointer32) == SUCCESS; zend_hash_move_forward_ex(arr_hash32, &pointer32)) {
                       
                    //zval temp;
                    zend_string *key2;
//                  uint key2_len;
                    ulong index2;
                    ZEND_HASH_FOREACH_KEY_VAL(arr_hash32, index2, key2, data32){ 
//                        if (zend_hash_get_current_key_ex(arr_hash32, &key2, &key2_len, &index2, 0, &pointer32) != HASH_KEY_NON_EXISTANT) {
                        if(key2){    
                            if (strlen(key2->val) == 10){
                                //std::cerr<<"date ";
                                std::string temp32(Z_STRVAL_P(data32));
                                date.assign(key2->val);
                                day.assign(temp32);
                                //std::cerr<<date<<" "<<day<<std::endl;
                                loan_transfer_daychange__info[date]=day;
                            }
                        }

                    }ZEND_HASH_FOREACH_END();
                    ////exit(1);
                }
                else if (strcmp(key->val, "samity_transfers") == 0) {

                    //std::cerr<<"samity_transfers\n";
                    zval temp3;

                    zval *data33;
                    HashTable *arr_hash33;

                    temp3 = *data31;
                    zval_copy_ctor(&temp3);
                    convert_to_array(&temp3);

                    arr_hash33 = Z_ARRVAL_P(&temp3);
//                    array_count33 = zend_hash_num_elements(arr_hash33);
                    
                    std::string date;
                    int samity;

//                    for (zend_hash_internal_pointer_reset_ex(arr_hash33, &pointer33); zend_hash_get_current_data_ex(arr_hash33, (void**) &data33, &pointer33) == SUCCESS; zend_hash_move_forward_ex(arr_hash33, &pointer33)) {
                    
                        //zval temp;
                        zend_string *key2;
//                        uint key2_len;
                        ulong index2;
                    ZEND_HASH_FOREACH_KEY_VAL(arr_hash33, index2, key2, data33){
//                        if (zend_hash_get_current_key_ex(arr_hash33, &key2, &key2_len, &index2, 0, &pointer33) != HASH_KEY_NON_EXISTANT) {
                        if(key2){    
                            if (strlen(key2->val) == 10){
                                //std::cerr<<"date ";
                                std::string temp33(Z_STRVAL_P(data33));
                                date.assign(key2->val);
                                samity=atoi(temp33.c_str());
                                //std::cerr<<date<<" "<<samity<<std::endl;
                                loan_transfer_samitychange__info[date]=samity;
                            }
                        }

                    }ZEND_HASH_FOREACH_END();
                    ////exit(1);
                }
                else if (strcmp(key->val, "one_time_weekly_loans_transactions") == 0) {

                    //std::cerr<<"one_time_weekly_loans_transactions\n";
                    zval temp4;

                    zval *data34;
                    HashTable *arr_hash34;

                    temp4 = *data31;
                    zval_copy_ctor(&temp4);
                    convert_to_array(&temp4);

                    arr_hash34 = Z_ARRVAL_P(&temp4);
                    
                    std::string date;
                    double one_time_weekly_loans_transaction;

//                    for (zend_hash_internal_pointer_reset_ex(arr_hash34, &pointer34); zend_hash_get_current_data_ex(arr_hash34, (void**) &data34, &pointer34) == SUCCESS; zend_hash_move_forward_ex(arr_hash34, &pointer34)) {
                       
                        //zval temp;
                        zend_string *key2;
//                        uint key2_len;
                        ulong index2;
                    ZEND_HASH_FOREACH_KEY_VAL(arr_hash34, index2, key2, data34){ 
//                        if (zend_hash_get_current_key_ex(arr_hash34, &key2, &key2_len, &index2, 0, &pointer34) != HASH_KEY_NON_EXISTANT) {
                        if(key2){    
                            if (strlen(key2->val) == 10){
                                //std::cerr<<"date ";
                                std::string temp34(Z_STRVAL_P(data34));
                                date.assign(key2->val);
                                one_time_weekly_loans_transaction = strtod(temp34.c_str(), NULL);
                                //std::cerr<<date<<" "<<one_time_weekly_loans_transaction<<std::endl;
                                one_time_weekly_loans_transactions__info[date]=one_time_weekly_loans_transaction;
                            }
                        }

                    }ZEND_HASH_FOREACH_END();
                    ////exit(1);
                }
                else if (strcmp(key->val, "penalty") == 0) {
                    //std::cerr<<"penalty ";
                    std::string temp1(Z_STRVAL_P(data31));
                    penalty = strtod(temp1.c_str(), NULL);

                }
                else if (strcmp(key->val, "rebate") == 0) {
                    //std::cerr<<"rebate ";
                    std::string temp1(Z_STRVAL_P(data31));
                    rebate = strtod(temp1.c_str(), NULL);

                }
                else if (strcmp(key->val, "rebate_date") == 0) {
                    //std::cerr<<"rebate ";
                    rebate_date = boost::gregorian::from_simple_string(Z_STRVAL_P(data31));
                    
                }
                else if (strcmp(key->val, "waiver") == 0) {
                    //std::cerr<<"waiver ";
                    std::string temp1(Z_STRVAL_P(data31));
                    waiver = strtod(temp1.c_str(), NULL);

                    //std::cerr<<"waiver "<<waiver<<std::endl;
                    //ll.SetInterest_rate(interest_rate);

                }                    
                else if (strcmp(key->val, "interest_payment_frequency") == 0) {
                    //std::cerr<<"interest_payment_frequency ";
                    std::string temp1(Z_STRVAL_P(data31));
                    interest_payment_frequency.assign(temp1.c_str());

                    //std::cerr<<"waiver "<<interest_payment_frequency<<std::endl;
                    //ll.SetInterest_rate(interest_rate);

                }
                else if (strcmp(key->val, "repayment_mode_for_monthly_loan") == 0) {
                    //std::cerr<<"repayment_mode_for_monthly_loan ";
                    std::string temp1(Z_STRVAL_P(data31));
                    repayment_mode_for_monthly_loan.assign(temp1.c_str());

                    //std::cerr<<"repayment_mode_for_monthly_loan "<<repayment_mode_for_monthly_loan<<std::endl;
                    //ll.SetInterest_rate(interest_rate);

                }
                else if (strcmp(key->val, "service_charge_taken_time") == 0) {
                    //std::cerr<<"repayment_mode_for_monthly_loan ";
                    //service_charge_taken_time = std::string(Z_STRVAL_P(data31));
                    
                    std::string temp1(Z_STRVAL_P(data31));
                    service_charge_taken_time.assign(temp1.c_str());
                }
               
                else if (strcmp(key->val, "repayment_day_or_week") == 0) {
                    //std::cerr<<"repayment_day_or_week ";
                    std::string temp1(Z_STRVAL_P(data31));
                    repayment_day_or_week.assign(temp1.c_str());
                }
                else if (strcmp(key->val, "interest_index") == 0) {
                    //std::cerr<<"interest_index ";
                    std::string temp1(Z_STRVAL_P(data31));
                    interest_index = strtod(temp1.c_str(), NULL);
                }
                else if (strcmp(key->val, "enforce_number_of_installment") == 0) {
                     //std::cerr<<"enforce_number_of_installment ";
                    std::string temp1(Z_STRVAL_P(data31));
                    enforce_number_of_installment = atoi(temp1.c_str());
                }
                else if (strcmp(key->val, "enforced_install_number") == 0) {                    
                    std::string temp1(Z_STRVAL_P(data31));
                    enforced_install_number = atoi(temp1.c_str());
                }
                else if (strcmp(key->val, "is_show_rec_after_loan_period") == 0) {                     
                    std::string temp1(Z_STRVAL_P(data31));
                    is_show_rec_after_loan_period = atoi(temp1.c_str());                     
                }
                else if (strcmp(key->val, "cycle") == 0) {                     
                    std::string temp1(Z_STRVAL_P(data31));
                    cycle = atoi(temp1.c_str());                     
                }
                else if (strcmp(key->val, "installment_amount") == 0) {                    
                    std::string temp1(Z_STRVAL_P(data31));
                    installment_amountdb = strtod(temp1.c_str(), NULL); 
                    //std::cerr<<"installment_amountdb "<<installment_amountdb<<std::endl;
                }
               
            }

        } ZEND_HASH_FOREACH_END();
      
        loan_list[ID] = new loan(
                one_time_weekly_loans_transactions__info,
                loan_transfer_samitychange__info,
                loan_transfer_daychange__info,
                ID, branch_id, samity_id, product_id, purpose_id,member_id, loan_amount, interest_rate, number_of_installment, repayment_frequency,grace_period, loan_period_in_month,
                first_repayment_date, last_repayment_date, disburse_date, loan_fully_paid_date, current_status, is_loan_fully_paid, is_authorized,interest_calculation_method,mode_of_interest,loan_closing_date,extra_installment_amount,
                original_loan_id,is_transferred_in,transfer_in_date,is_transferred_out,transfer_out_date,
                penalty,rebate,rebate_date,waiver,loan_type,system_date,
                interest_payment_frequency,repayment_mode_for_monthly_loan,repayment_day_or_week,
                interest_index,enforce_number_of_installment,enforced_install_number,
                decline_period,is_show_rec_after_loan_period,installment_amountdb,loan_period_in_day,is_loan_period_in_day_fixed,skip_last_week_calculation_monthly,service_charge_taken_time,cycle
                
                );
        loan_list[ID];
    } ZEND_HASH_FOREACH_END();
//    std::cerr<<"CC In reschedules"<<std::endl;
    std::vector<date_pair> reschedules;

    typedef __gnu_cxx::hash_map<int, std::vector<date_pair> > reschedule_hash_type;

    reschedule_hash_type reschedule_hash;

//    for (zend_hash_internal_pointer_reset_ex(arr_hash4, &pointer4); zend_hash_get_current_data_ex(arr_hash4, (void**) &data4, &pointer4) == SUCCESS; zend_hash_move_forward_ex(arr_hash4, &pointer4)) {
        zend_string *key4;
//        uint key_len;
        ulong index4;
    ZEND_HASH_FOREACH_KEY_VAL(arr_hash4, index4, key4, data4){    

        std::vector<date_pair> loan_reschedules;

//        if (zend_hash_get_current_key_ex(arr_hash4, &key, &key_len, &index, 0, &pointer4) != HASH_KEY_NON_EXISTANT) {
        if(key4){    
            //std::cout<<std::endl<<"in key "<<key<<" "<<index<<" in key "<<Z_STRVAL_P(data4)<<std::endl;

            zval temp;

            zval *data41;
            HashTable *arr_hash41;

            temp = *data4;
            zval_copy_ctor(&temp);
            convert_to_array(&temp);

            arr_hash41 = Z_ARRVAL_P(&temp);
//            array_count41 = zend_hash_num_elements(arr_hash41);


            std::string start;
            std::string end;

//            for (zend_hash_internal_pointer_reset_ex(arr_hash41, &pointer41); zend_hash_get_current_data_ex(arr_hash41, (void**) &data41, &pointer41) == SUCCESS; zend_hash_move_forward_ex(arr_hash41, &pointer41)) {
            ZEND_HASH_FOREACH_VAL(arr_hash41, data41){    
                zval *data42;
                HashTable *arr_hash42;

                zval temp1;

                temp1 = *data41;
                zval_copy_ctor(&temp1);
                convert_to_array(&temp1);

                arr_hash42 = Z_ARRVAL_P(&temp1);
//                array_count42 = zend_hash_num_elements(arr_hash42);

                boost::gregorian::date reschedule_from; // = boost::gregorian::from_simple_string(Z_STRVAL_P(data31));
                boost::gregorian::date reschedule_to; // = boost::gregorian::from_simple_string(Z_STRVAL_P(data31));

//                for (zend_hash_internal_pointer_reset_ex(arr_hash42, &pointer42); zend_hash_get_current_data_ex(arr_hash42, (void**) &data42, &pointer42) == SUCCESS; zend_hash_move_forward_ex(arr_hash42, &pointer42)) {
                zend_string *key1;
//              uint key_len1;
                ulong index1;
                ZEND_HASH_FOREACH_KEY_VAL(arr_hash42, index1, key1, data42){    

//                    if (zend_hash_get_current_key_ex(arr_hash42, &key1, &key_len1, &index1, 0, &pointer42) != HASH_KEY_NON_EXISTANT) {
                    if(key1){    
                        //std::cout<<std::endl<<key1<<" Key Value "<<Z_STRVAL_P(data42)<<std::endl;

                        if (strcmp(key1->val, "reschedule_from") == 0) {

                            std::cout << std::endl << std::string(Z_STRVAL_P(data42)) << std::endl;
                            reschedule_from = boost::gregorian::from_simple_string(std::string(Z_STRVAL_P(data42)));


                        } else if (strcmp(key1->val, "reschedule_to") == 0) {
                            reschedule_to = boost::gregorian::from_simple_string(std::string(Z_STRVAL_P(data42)));
                        }
                    }
                }ZEND_HASH_FOREACH_END();
                //std::cout<<std::endl<<"in 400"<<std::endl;
                date_pair r_dp(reschedule_from, reschedule_to);
                loan_reschedules.push_back(r_dp);
                //std::cout<<std::endl<<"in 403"<<std::endl;

            }ZEND_HASH_FOREACH_END();
        }
        reschedule_hash[index4] = loan_reschedules;

    }ZEND_HASH_FOREACH_END();

    /*
    for(reschedule_hash_type::iterator it=reschedule_hash.begin();it!=reschedule_hash.end();it++){
            std::cout<<"loan_id: "<<it->first<<std::endl;
            for(std::vector<date_pair>::iterator it1=it->second.begin();it1!=it->second.end();it1++){
                    std::cout<<"Begin: "<<it1->start<<" End: "<<it1->end<<std::endl;
            }
    }
     */


//    std::cerr<<"CC In samities"<<std::endl;
    //std::vector<samity> uu_samities;
    typedef __gnu_cxx::hash_map<int, samity*> samity_hash;
    samity_hash uu_samities;
//    for (zend_hash_internal_pointer_reset_ex(arr_hash5, &pointer5); zend_hash_get_current_data_ex(arr_hash5, (void**) &data5, &pointer5) == SUCCESS; zend_hash_move_forward_ex(arr_hash5, &pointer5)) {
    ZEND_HASH_FOREACH_VAL(arr_hash5, data5){    
        zval temp;

        zval *data51;
        HashTable *arr_hash51;

        temp = *data5;
        zval_copy_ctor(&temp);
        convert_to_array(&temp);

        arr_hash51 = Z_ARRVAL_P(&temp);
//        array_count51 = zend_hash_num_elements(arr_hash51);

        samity *ss=new samity();
        int ID;

//        for (zend_hash_internal_pointer_reset_ex(arr_hash51, &pointer51); zend_hash_get_current_data_ex(arr_hash51, (void**) &data51, &pointer51) == SUCCESS; zend_hash_move_forward_ex(arr_hash51, &pointer51)) {
            zend_string *key;
//            uint key_len;
            ulong index;
        ZEND_HASH_FOREACH_KEY_VAL(arr_hash51, index, key, data51){    
            // php_printf("\n");
            zval temp;

//            if (zend_hash_get_current_key_ex(arr_hash51, &key, &key_len, &index, 0, &pointer51) != HASH_KEY_NON_EXISTANT) {
            if(key){    
                if (strcmp(key->val, "id") == 0) {
                    //std::cerr<<"id ";
                    std::string temp1(Z_STRVAL_P(data51));
                    int temp = atoi(temp1.c_str());

                    ss->SetId(temp);
                    ID = temp;
                    
                    //std::cerr<<ID<<std::endl;

                } else if (strcmp(key->val, "branch_id") == 0) {

                    //std::cerr<<"branch_id ";
                    std::string temp1(Z_STRVAL_P(data51));
                    int temp = atoi(temp1.c_str());

                    ss->SetBranch_id(temp);
                    //std::cerr<<temp<<std::endl;

                } else if (strcmp(key->val, "samity_type") == 0) {

                    //std::cerr<<"samity_type ";
                    std::string temp1(Z_STRVAL_P(data51));
                    //int temp=atoi(temp1.c_str());

                    ss->SetSamity_type(temp1);
                    //std::cerr<<temp1<<std::endl;

                } else if (strcmp(key->val, "opening_date") == 0) {
                    //std::cerr<<"opening_date ";
                    std::string temp1(Z_STRVAL_P(data51));
                    //int temp=atoi(temp1.c_str());
                    ss->SetOpening_date(boost::gregorian::from_simple_string(temp1));
                    //std::cerr<<temp1<<std::endl;

                }
                else if (strcmp(key->val, "samity_day_change_flag") == 0) {
                    //std::cerr<<"samity_day_change_flag ";
                    std::string temp1(Z_STRVAL_P(data51));
                    //int temp=atoi(temp1.c_str());
                    //std::cerr<<temp1<<std::endl;
                    ////exit(1);
                    ss->samity_day_change=atoi(temp1.c_str());
                    //std::cerr<<temp1<<std::endl;

                }
                else if (strcmp(key->val, "is_samity_day_obsolete") == 0) {                    
                    std::string temp1(Z_STRVAL_P(data51));                    
                    ss->is_samity_day_obsolete=atoi(temp1.c_str());
                }
                else if (strcmp(key->val, "max_min_schedule_dates") == 0) {

                    //std::cerr<<"max_min_schedule_dates ";
                    zval temp3;

                    zval *data53;
                    HashTable *arr_hash53;

                    temp3 = *data51;
                    zval_copy_ctor(&temp3);
                    convert_to_array(&temp3);

                    arr_hash53 = Z_ARRVAL_P(&temp3);
//                    array_count53 = zend_hash_num_elements(arr_hash53);
                    
                    std::string start;
                    std::string end;

//                    for (zend_hash_internal_pointer_reset_ex(arr_hash53, &pointer53); zend_hash_get_current_data_ex(arr_hash53, (void**) &data53, &pointer53) == SUCCESS; zend_hash_move_forward_ex(arr_hash53, &pointer53)) {
                    zend_string *key2;
//                  uint key2_len;
                    ulong index2;
                    ZEND_HASH_FOREACH_KEY_VAL(arr_hash53, index2, key2, data53){    
                        //zval temp;

//                        if (zend_hash_get_current_key_ex(arr_hash53, &key2, &key2_len, &index2, 0, &pointer53) != HASH_KEY_NON_EXISTANT) {
                        if(key2){    
                            if (strcmp(key2->val, "start") == 0){
                                //std::cerr<<"start ";
                                std::string temp53(Z_STRVAL_P(data53));
                                start.assign( temp53);
                                //std::cerr<<temp53<<std::endl;
                            }
                            else if (strcmp(key2->val, "end") == 0){
                                //std::cerr<<"end ";
                                std::string temp53(Z_STRVAL_P(data53));
                                end.assign( temp53);
                                 //std::cerr<<temp53<<std::endl;
                            }
                        }

                    }ZEND_HASH_FOREACH_END();
                  
                    if(samity_holiday_list_s->find(ID)!=samity_holiday_list_s->end()){
                        //std::cerr<<"Making Samity Schedule With Samity Holiday\n"<<start<<" "<<end<<std::endl;
                        ss->make_samity_schedule(boost::gregorian::from_simple_string(start),boost::gregorian::from_simple_string(end),b_holiday_s,(*samity_holiday_list_s)[ID]);
                        //std::cerr<<"Making Samity Schedule With Samity Holiday END\n"<<start<<" "<<end<<std::endl;
                        
                    }
                    else{
                        //std::cerr<<"Making Samity Schedule Without Samity Holiday\n";
                        ss->make_samity_schedule(boost::gregorian::from_simple_string(start),boost::gregorian::from_simple_string(end),b_holiday_s);
                         //std::cerr<<"Making Samity Schedule Without Samity Holiday END\n";
                    }
                }
                else if (strcmp(key->val, "samity_days") == 0) {

                    //std::cerr<<"samity_days ";
                    zval temp2;

                    zval *data52;
                    HashTable *arr_hash52;

                    temp2 = *data51;
                    zval_copy_ctor(&temp2);
                    convert_to_array(&temp2);

                    arr_hash52 = Z_ARRVAL_P(&temp2);
//                    array_count52 = zend_hash_num_elements(arr_hash52);

//                    for (zend_hash_internal_pointer_reset_ex(arr_hash52, &pointer52); zend_hash_get_current_data_ex(arr_hash52, (void**) &data52, &pointer52) == SUCCESS; zend_hash_move_forward_ex(arr_hash52, &pointer52)) {
                        zend_string *key1;
//                        uint key1_len;
                        ulong index1;                    
                    ZEND_HASH_FOREACH_KEY_VAL(arr_hash52, index1, key1, data52){    
                        //zval temp;

//                        if (zend_hash_get_current_key_ex(arr_hash52, &key1, &key1_len, &index1, 0, &pointer52) != HASH_KEY_NON_EXISTANT) {
                        if(key1){    
                            //std::cerr<<"samity_days ";
                            ss->add_samity_days(Z_STRVAL_P(data52), key1->val);

                            ss->set_samity_day(Z_STRVAL_P(data52));
                        }

                    }ZEND_HASH_FOREACH_END();
                  
                }
            }

            uu_samities[ID] = ss;
        }ZEND_HASH_FOREACH_END();
    }ZEND_HASH_FOREACH_END();
//    std::cerr<<"System Date "<<sys_date<<"\n";
//    exit(1);
    if (sch != NULL) {
        sch->initialize(system_date,b_id, s_id, d_frm, d_to, b_holiday_s, samity_holiday_list, reschedule_hash, uu_samities, loan_list,org_flag);
    }
    
}

PHP_METHOD(Loan_scheduler, reinitialize) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    zval *data4;
    HashTable *arr_hash4;


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "h", &arr_hash4) == FAILURE) {
        return;
    }

    std::vector<date_pair> reschedules;

    typedef __gnu_cxx::hash_map<int, std::vector<date_pair> > reschedule_hash_type;

    reschedule_hash_type reschedule_hash;

//    for (zend_hash_internal_pointer_reset_ex(arr_hash4, &pointer4); zend_hash_get_current_data_ex(arr_hash4, (void**) &data4, &pointer4) == SUCCESS; zend_hash_move_forward_ex(arr_hash4, &pointer4)) {

        zend_string *key;
//        uint key_len;
        ulong index;
    ZEND_HASH_FOREACH_KEY_VAL(arr_hash4, index, key, data4){    
        std::vector<date_pair> loan_reschedules;

//        if (zend_hash_get_current_key_ex(arr_hash4, &key, &key_len, &index, 0, &pointer4) != HASH_KEY_NON_EXISTANT) {
        if(key){    
            //std::cout << std::endl << "in key " << key << " " << index << " in key " << Z_STRVAL_P(data4) << std::endl;

            zval temp;

            zval *data41;
            HashTable *arr_hash41;

            temp = *data4;
            zval_copy_ctor(&temp);
            convert_to_array(&temp);

            std::string start;
            std::string end;

//            for (zend_hash_internal_pointer_reset_ex(arr_hash41, &pointer41); zend_hash_get_current_data_ex(arr_hash41, (void**) &data41, &pointer41) == SUCCESS; zend_hash_move_forward_ex(arr_hash41, &pointer41)) {
            ZEND_HASH_FOREACH_VAL(arr_hash41, data41){    
                zval *data42;
                HashTable *arr_hash42;

                zval temp1;

                temp1 = *data41;
                zval_copy_ctor(&temp1);
                convert_to_array(&temp1);

                boost::gregorian::date reschedule_from; // = boost::gregorian::from_simple_string(Z_STRVAL_P(data31));
                boost::gregorian::date reschedule_to; // = boost::gregorian::from_simple_string(Z_STRVAL_P(data31));

//                for (zend_hash_internal_pointer_reset_ex(arr_hash42, &pointer42); zend_hash_get_current_data_ex(arr_hash42, (void**) &data42, &pointer42) == SUCCESS; zend_hash_move_forward_ex(arr_hash42, &pointer42)) {



                    zend_string *key1;
//                    uint key_len1;
                    ulong index1;
                ZEND_HASH_FOREACH_KEY_VAL(arr_hash42, index1, key1, data42){    
//                    if (zend_hash_get_current_key_ex(arr_hash42, &key1, &key_len1, &index1, 0, &pointer42) != HASH_KEY_NON_EXISTANT) {
                    if(key1){    
                        //std::cout << std::endl << key1 << " Key Value " << Z_STRVAL_P(data42) << std::endl;

                        if (strcmp(key1->val, "reschedule_from") == 0) {

                            std::cout << std::endl << std::string(Z_STRVAL_P(data42)) << std::endl;
                            reschedule_from = boost::gregorian::from_simple_string(std::string(Z_STRVAL_P(data42)));


                        } else if (strcmp(key1->val, "reschedule_to") == 0) {
                            reschedule_to = boost::gregorian::from_simple_string(std::string(Z_STRVAL_P(data42)));

                        }
                    }
                }ZEND_HASH_FOREACH_END();
                //std::cout << std::endl << "in 400" << std::endl;
                date_pair r_dp(reschedule_from, reschedule_to);
                loan_reschedules.push_back(r_dp);
                //std::cout << std::endl << "in 403" << std::endl;

            }ZEND_HASH_FOREACH_END();
            //std::cout << std::endl << "in 406" << std::endl;
            //date_pair dp("2011-01-01", "2011-01-01");
            //reschedules.push_back(dp);
            //std::cout << std::endl << "in 409" << std::endl;
        }
        reschedule_hash[index] = loan_reschedules;

    }ZEND_HASH_FOREACH_END();

    if (sch != NULL) {
        sch->reinitialize(reschedule_hash);
    }

}

PHP_METHOD(Loan_scheduler, set_system_date) {
    
    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;
    
    char *sys_date;
    int sys_date_len;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &sys_date, &sys_date_len) == FAILURE) {
        return;
    }
    std::string system_date(sys_date);
    if (sch != NULL) {
        sch->set_system_date(system_date);
    }
}

PHP_METHOD(Loan_scheduler, get_first_schedule_date) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    long l_id;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &l_id) == FAILURE) {
        return;
    }
    std::string first_sch_date;
    if (sch != NULL) {
        first_sch_date = sch->get_first_schedule_date(l_id);
    }

    RETURN_STRING(first_sch_date.c_str());
}

PHP_METHOD(Loan_scheduler, get_last_schedule_date) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    long l_id;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &l_id) == FAILURE) {
        return;
    }
    std::string last_sch_date;
    if (sch != NULL) {
        last_sch_date = sch->get_last_schedule_date(l_id);
    }

    RETURN_STRING(last_sch_date.c_str());
}

PHP_METHOD(Loan_scheduler, get_schedule_date_by_installment_no) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    long l_id;
    long ins_no;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &l_id, &ins_no) == FAILURE) {
        return;
    }
    std::string sch_date;
    if (sch != NULL) {
        sch_date = sch->get_schedule_date_by_installment_no(l_id, ins_no);
    }

    RETURN_STRING(sch_date.c_str());
}

PHP_METHOD(Loan_scheduler, get_schedule_installment_info_by_installment_no) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    zend_long l_id;
    zend_long ins_no;
    zval subarray;
    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &l_id, &ins_no) == FAILURE) {
        return;
    }
    std::string sch_date;
    if (sch != NULL) {
        subarray = sch->get_schedule_installment_info_by_installment_no(l_id, ins_no);
    }

    add_index_zval(return_value, 0, &subarray);
}

PHP_METHOD(Loan_scheduler, get_loan_recoverable_amount) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    zend_long branch_id;
    zend_long product_id;
    char* date;
    size_t date_len;
    zval subarray;
    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lls", &branch_id, &product_id, &date, &date_len) == FAILURE) {
        return;
    }

    if (sch != NULL) {
        subarray = sch->get_loan_recoverable_amount(branch_id, product_id, date);
    }
    //RETVAL_RESOURCE(subarray);
    add_index_zval(return_value, 0, &subarray);
    // return subarray;
}

PHP_METHOD(Loan_scheduler, get_loan_recoverable_amount_branch_id) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    zend_long branch_id;
    char* s_date_frm;
    size_t s_date_frm_len;
    char* s_date_to=NULL;
    size_t s_date_to_len;
    zval subarray;
    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls|s", &branch_id, &s_date_frm, &s_date_frm_len, &s_date_to, &s_date_to_len) == FAILURE) {
        return;
    }
    std::string date_frm(s_date_frm);
    std::string date_to="";
    if(s_date_to)
        date_to.assign(s_date_to);

    if (sch != NULL) {
        subarray = sch->get_loan_recoverable_amount_branch_id(branch_id, date_frm, date_to);
    }
    //RETVAL_RESOURCE(subarray);
    //add_index_zval(return_value, subarray);
    //add_assoc_zval(return_value, "0", subarray);
    //add_assoc_long(return_value, "life", 42);
    add_index_zval(return_value, 0, &subarray);
    // return subarray;
}


PHP_METHOD(Loan_scheduler, get_branch_wise_loan_recoverable_amount_by_product_id) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    zend_long branch_id;
    char* s_date_frm;
    size_t s_date_frm_len;
    char* s_date_to=NULL;
    size_t s_date_to_len;
    zval subarray;
    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls|s", &branch_id, &s_date_frm, &s_date_frm_len, &s_date_to, &s_date_to_len) == FAILURE) {
        return;
    }
    
    std::string date_frm(s_date_frm);
    std::string date_to="";
    if(s_date_to)
        date_to.assign(s_date_to);
    if (sch != NULL) {
        subarray = sch->get_branch_wise_loan_recoverable_amount_by_product_id(branch_id, date_frm, date_to);
    }
    //RETVAL_RESOURCE(subarray);   
    add_index_zval(return_value, 0, &subarray);
    // return subarray;
}
PHP_METHOD(Loan_scheduler, get_branch_wise_loan_recoverable_amount) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    zend_long branch_id;
    char* s_date_frm=NULL;
    size_t s_date_frm_len;
    char* s_date_to=NULL;
    size_t s_date_to_len;
    zval subarray;
    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls|s", &branch_id, &s_date_frm, &s_date_frm_len, &s_date_to, &s_date_to_len) == FAILURE) {
        return;
    }
    //std::cerr<<"get_branch_wise_loan_recoverable_amount\n";
    std::string date_frm(s_date_frm);
    std::string date_to="";
    if(s_date_to)
        date_to.assign(s_date_to);
    
    if (sch != NULL) {
        subarray = sch->get_branch_wise_loan_recoverable_amount(branch_id, date_frm, date_to);
    }
    //RETVAL_RESOURCE(subarray);   
    add_index_zval(return_value, 0, &subarray);
    // return subarray;
}


PHP_METHOD(Loan_scheduler, get_branch_wise_loan_recoverable_amount_consider_fully_paid) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    zend_long branch_id;
    char* s_date_frm=NULL;
    size_t s_date_frm_len;
    char* s_date_to=NULL;
    size_t s_date_to_len;
    zval subarray;
    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls|s", &branch_id, &s_date_frm, &s_date_frm_len, &s_date_to, &s_date_to_len) == FAILURE) {
        return;
    }
    //std::cerr<<"get_branch_wise_loan_recoverable_amount\n";
    std::string date_frm(s_date_frm);
    std::string date_to="";
    if(s_date_to)
        date_to.assign(s_date_to);
    
    if (sch != NULL) {
        subarray = sch->get_branch_wise_loan_recoverable_amount_consider_fully_paid(branch_id, date_frm, date_to);
    }
    //RETVAL_RESOURCE(subarray);   
    add_index_zval(return_value, 0, &subarray);
    // return subarray;
}

PHP_METHOD(Loan_scheduler, get_loan_schedules) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    long branch_id = -1;
    long samity_id = -1;
    long member_id = -1;
    long loan_id = -1;


    zval subarray;
    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|lll", &branch_id, &samity_id, &member_id, &loan_id) == FAILURE) {
        return;
    }

    if (sch != NULL) {
        subarray = sch->get_loan_schedules(branch_id, samity_id, member_id, loan_id);
    }
    //std::cerr<<"CC In Loan Schedules"<<std::endl;
    add_index_zval(return_value, 0, &subarray);
    //std::cerr<<"After Return"<<std::endl;
}

PHP_METHOD(Loan_scheduler, get_loan_schedules_samity_wise) {

Loan_scheduler *sch;
//Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
sch = obj->sch;

zend_long branch_id = -1;
zend_long samity_id = -1; 


zval subarray;
array_init(return_value);

if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &branch_id, &samity_id) == FAILURE) {
return;
}

if (sch != NULL) {
subarray = sch->get_loan_schedules_samity_wise(branch_id, samity_id);
}
//std::cerr<<"CC In Loan Schedules"<<std::endl;
add_index_zval(return_value, 0, &subarray);
//std::cerr<<"After Return"<<std::endl;
}

PHP_METHOD(Loan_scheduler, get_full_loan_schedules) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    zend_long branch_id = -1;
    zend_long samity_id = -1;
    zend_long member_id = -1;
    zend_long loan_id = -1;


    zval subarray;
    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &branch_id, &samity_id, &member_id, &loan_id) == FAILURE) {
        return;
    }

    if (sch != NULL) {
        subarray = sch->get_full_loan_schedules(branch_id, samity_id, member_id, loan_id);
    }
    //std::cerr<<"CC In Loan Schedules"<<std::endl;
    add_index_zval(return_value, 0, &subarray);
    //std::cerr<<"After Return"<<std::endl;
}


PHP_METHOD(Loan_scheduler, get_loan_schedules_by_memeberlist_branch_product) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    std::vector<int> member_list;
    zend_long branch_id;
    zend_long product_id;
    zend_long samity_id = -1;

    std::string s_date_frm = "";
    std::string s_date_to = "";

    zval *data1;
    HashTable *arr_hash1;

    char *sc_date_frm;
    size_t sc_date_frm_len = 0;

    char *sc_date_to;
    size_t sc_date_to_len = 0;

    zval subarray;
    array_init(return_value);


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hll|lss", &arr_hash1, &branch_id, &product_id, &samity_id, &sc_date_frm, &sc_date_frm_len, &sc_date_to, &sc_date_to_len) == FAILURE) {
        return;
    }

    if (sc_date_frm_len)
        s_date_frm = std::string(sc_date_frm);
    if (sc_date_to_len)
        s_date_to = std::string(sc_date_to);



//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {
    ZEND_HASH_FOREACH_VAL(arr_hash1, data1){    
        if (Z_TYPE_P(data1) == IS_LONG) {
            member_list.push_back(Z_LVAL_P(data1));
        } else {
            std::string temp1(Z_STRVAL_P(data1));
            int temp = atoi(temp1.c_str());
            member_list.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();

    if (sch != NULL) {
        subarray = sch->get_loan_schedules_by_memeberlist_branch_product(member_list, branch_id, product_id, samity_id, s_date_frm, s_date_to);
    }

    add_index_zval(return_value, 0, &subarray);
}

PHP_METHOD(Loan_scheduler, get_member_wise_loan_schedules_by_branch) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    std::vector<int> member_list;
    zend_long branch_id;
    

    std::string s_date_frm = "";
    std::string s_date_to = "";

    zval *data1;
    HashTable *arr_hash1;

    char *sc_date_frm;
    size_t sc_date_frm_len = 0;

    char *sc_date_to;
    size_t sc_date_to_len = 0;

    zval subarray;
    array_init(return_value);


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hl|ss", &arr_hash1, &branch_id, &sc_date_frm, &sc_date_frm_len, &sc_date_to, &sc_date_to_len) == FAILURE) {
        return;
    }

    if (sc_date_frm_len)
        s_date_frm = std::string(sc_date_frm);
    if (sc_date_to_len)
        s_date_to = std::string(sc_date_to);



//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {
    ZEND_HASH_FOREACH_VAL(arr_hash1, data1){    
        if (Z_TYPE_P(data1) == IS_LONG) {
            member_list.push_back(Z_LVAL_P(data1));
        } else {
            std::string temp1(Z_STRVAL_P(data1));
            int temp = atoi(temp1.c_str());
            member_list.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();

    if (sch != NULL) {
        subarray = sch->get_member_wise_loan_schedules_by_branch(member_list, branch_id, s_date_frm, s_date_to);
    }

    add_index_zval(return_value, 0, &subarray);
}

PHP_METHOD(Loan_scheduler, get_active_member_loan_schedule_info) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    std::vector<int> member_list;
    std::string s_date;
    zend_long branch_id;
    zend_long product_id;
    zend_long samity_id = -1;

    zval *data1;
    HashTable *arr_hash1;

    char *sc_date;
    size_t sc_date_len;

    zval subarray;
    array_init(return_value);


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hsll|l", &arr_hash1, &sc_date, &sc_date_len, &branch_id, &product_id, &samity_id) == FAILURE) {
        return;
    }

    s_date = std::string(sc_date);
//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {
    ZEND_HASH_FOREACH_VAL(arr_hash1, data1){    
        if (Z_TYPE_P(data1) == IS_LONG) {
            member_list.push_back(Z_LVAL_P(data1));
        } else {
            std::string temp1(Z_STRVAL_P(data1));
            int temp = atoi(temp1.c_str());
            member_list.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();

    if (sch != NULL) {
        subarray = sch->get_active_member_loan_schedule_info(member_list, s_date, branch_id, product_id, samity_id);
    }

    add_index_zval(return_value, 0, &subarray);
}


PHP_METHOD(Loan_scheduler, get_branch_wise_loan_information_before_date) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;
    //std::cerr<< "get_branch_wise_loan_information_before_date\n";
    
    std::string s_date;
    zend_long branch_id;
    std::vector<int> samity_list=std::vector<int>();
    zend_long product_id=0;
    zend_long loan_id=0;
    
    zval *data1;
    HashTable *arr_hash1;

    char *sc_date;
    size_t sc_date_len;

    zval subarray;
    array_init(return_value);
    
    zend_bool include_fully_paid = 1;

    //std::cerr<< "before get_branch_wise_loan_information_before_date\n";
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls|hllb", &branch_id, &sc_date, &sc_date_len, &arr_hash1,&product_id,&loan_id,&include_fully_paid) == FAILURE) {
        return;
    }
    //std::cerr<< "after get_branch_wise_loan_information_before_date\n";
    
    s_date = std::string(sc_date);
    
//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {
    ZEND_HASH_FOREACH_VAL(arr_hash1, data1) {    

        if (Z_TYPE_P(data1) == IS_LONG) {
            samity_list.push_back(Z_LVAL_P(data1));
        } else {
            std::string temp1(Z_STRVAL_P(data1));
            double temp = atoi(temp1.c_str());
            samity_list.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();


    if (sch != NULL) {
        subarray = sch->get_branch_wise_loan_information_before_date(branch_id, s_date,samity_list,product_id,loan_id,include_fully_paid);
    }

    add_index_zval(return_value, 0, &subarray);
}


PHP_METHOD(Loan_scheduler, get_branch_wise_loan_information_between_date) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;
    
    std::string s_date_frm;
    std::string s_date_to="";
    zend_long branch_id;     
    char *sc_date_frm;
    size_t sc_date_frm_len;
    
    char *sc_date_to;
    size_t sc_date_to_len;

    zval subarray;
    array_init(return_value);


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls|s", &branch_id, &sc_date_frm, &sc_date_frm_len, &sc_date_to, &sc_date_to_len) == FAILURE) {
        return;
    }


    s_date_frm = std::string(sc_date_frm);
    if(sc_date_to)
    s_date_to = std::string(sc_date_to);


    if (sch != NULL) {
        subarray = sch->get_branch_wise_loan_information_between_date(branch_id, s_date_frm,s_date_to);
    }

    add_index_zval(return_value, 0, &subarray);
}




PHP_METHOD(Loan_scheduler, get_member_wise_current_loan_information_just_before_date) {
    //std::cerr<<"CC get_member_wise_current_loan_information_just_before_date\n";
    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    std::vector<int> member_list;
    std::string s_date_frm="";
    std::string s_date_to="";
    zend_long branch_id;
    zend_long product_id;
    zend_long samity_id = -1;

    zval *data1;
    HashTable *arr_hash1;

    char *sc_date_frm;
    size_t sc_date_frm_len;
    
    char *sc_date_to=NULL;
    size_t sc_date_to_len;

    zval subarray;
    array_init(return_value);


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hsll|ls", &arr_hash1, &sc_date_frm, &sc_date_frm_len, &branch_id, &product_id, &samity_id, &sc_date_to, &sc_date_to_len) == FAILURE) {
        return;
    }
    
    s_date_frm = std::string(sc_date_frm);
    if(sc_date_to)
        s_date_to=std::string(sc_date_to);

    
    
//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {

    ZEND_HASH_FOREACH_VAL(arr_hash1, data1) {    
        if (Z_TYPE_P(data1) == IS_LONG) {
            member_list.push_back(Z_LVAL_P(data1));
        } else {
            std::string temp1(Z_STRVAL_P(data1));
            int temp = atoi(temp1.c_str());
            member_list.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();
   
    
    if (sch != NULL) {
        subarray = sch->get_member_wise_current_loan_information_just_before_date(member_list, s_date_frm, branch_id, product_id, samity_id,s_date_to);
    }

    add_index_zval(return_value, 0, &subarray);
}




PHP_METHOD(Loan_scheduler, get_samity_wise_expired_loan_information_before_date) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    std::vector<int> samity_list;
    std::string s_date;
    zend_long branch_id;
    zend_long product_id;
    zend_long samity_id = -1;

    zval *data1;
    HashTable *arr_hash1;

    char *sc_date;
    size_t sc_date_len;

    zval subarray;
    array_init(return_value);


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hsl|l", &arr_hash1, &sc_date, &sc_date_len, &branch_id, &product_id) == FAILURE) {
        return;
    }

    s_date = std::string(sc_date);

//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {

    ZEND_HASH_FOREACH_VAL(arr_hash1, data1) {
        if (Z_TYPE_P(data1) == IS_LONG) {
            samity_list.push_back(Z_LVAL_P(data1));
        } else {
            std::string temp1(Z_STRVAL_P(data1));
            int temp = atoi(temp1.c_str());
            samity_list.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();

    if (sch != NULL) {
        subarray = sch->get_samity_wise_expired_loan_information_before_date(samity_list, s_date, branch_id, product_id);
    }

    add_index_zval(return_value, 0, &subarray);
}


///////////

PHP_METHOD(Loan_scheduler, get_overdue_loans_by_branch_samity) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    std::vector<int> samity_list;
    std::string s_date;
    zend_long branch_id;
    zend_long product_id;
    zend_long samity_id = -1;

    zval *data1;
    HashTable *arr_hash1;

    char *sc_date;
    size_t sc_date_len;

    zval subarray;
    array_init(return_value);


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hsl|l", &arr_hash1, &sc_date, &sc_date_len, &branch_id, &product_id) == FAILURE) {
        return;
    }

    s_date = std::string(sc_date);



//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {

    ZEND_HASH_FOREACH_VAL(arr_hash1, data1) {    

        if (Z_TYPE_P(data1) == IS_LONG) {
            samity_list.push_back(Z_LVAL_P(data1));
        } else {
            std::string temp1(Z_STRVAL_P(data1));
            int temp = atoi(temp1.c_str());
            samity_list.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();

    if (sch != NULL) {
        subarray = sch->get_overdue_loans_by_branch_samity(samity_list, s_date, branch_id, product_id);
    }

    add_index_zval(return_value, 0, &subarray);
}

///////////////////////////////////////////////////////////////////////////////
PHP_METHOD(Loan_scheduler, get_loan_principle_interrest_distribution_by_loan_ids_trasaction_amounts) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    std::vector<int> loan_ids;
    std::vector<double> total_transaction_amounts;
   
    zval *data1;
    HashTable *arr_hash1;
    
    zval *data2;
    HashTable *arr_hash2;

    zval subarray;
    array_init(return_value);


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hh", &arr_hash1, &arr_hash2) == FAILURE) {
        return;
    }

//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {

    ZEND_HASH_FOREACH_VAL(arr_hash1, data1) {    

        if (Z_TYPE_P(data1) == IS_LONG) {
            loan_ids.push_back(Z_LVAL_P(data1));
        } else {
            std::string temp1(Z_STRVAL_P(data1));
            int temp = atoi(temp1.c_str());
            loan_ids.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();
    
//    for (zend_hash_internal_pointer_reset_ex(arr_hash2, &pointer2); zend_hash_get_current_data_ex(arr_hash2, (void**) &data2, &pointer2) == SUCCESS; zend_hash_move_forward_ex(arr_hash2, &pointer2)) {

    ZEND_HASH_FOREACH_VAL(arr_hash2, data2) {    

        if (Z_TYPE_P(data2) == IS_DOUBLE) {
            total_transaction_amounts.push_back(Z_DVAL_P(data2));
        } else {
            std::string temp1(Z_STRVAL_P(data2));
            double temp = strtod(temp1.c_str(),NULL);
            total_transaction_amounts.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();

    if (sch != NULL) {
        subarray = sch->get_loan_principle_interrest_distribution_by_loan_ids_trasaction_amounts(loan_ids,total_transaction_amounts);
    }

    add_index_zval(return_value, 0, &subarray);
}
///////////////////////////////////////////////////////////////////////////////

PHP_METHOD(Loan_scheduler, get_loan_advance_due_by_loan_ids_trasaction_amounts) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    std::vector<int> loan_ids;
    std::vector<double> total_transaction_amounts;

    zval *data1;
    HashTable *arr_hash1;
    
    zval *data2;
    HashTable *arr_hash2;

    char *sc_date_to;
    size_t sc_date_to_len;
    std::string s_date_to;
    
    zend_bool include_expired = 0;

    zval subarray;
    array_init(return_value);


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hhs|b", &arr_hash1, &arr_hash2, &sc_date_to, &sc_date_to_len,&include_expired) == FAILURE) {
        return;
    }

//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {

    ZEND_HASH_FOREACH_VAL(arr_hash1, data1) {    

        if (Z_TYPE_P(data1) == IS_LONG) {
            loan_ids.push_back(Z_LVAL_P(data1));
        } else {
            std::string temp1(Z_STRVAL_P(data1));
            int temp = atoi(temp1.c_str());
            loan_ids.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();
    
//    for (zend_hash_internal_pointer_reset_ex(arr_hash2, &pointer2); zend_hash_get_current_data_ex(arr_hash2, (void**) &data2, &pointer2) == SUCCESS; zend_hash_move_forward_ex(arr_hash2, &pointer2)) {
    ZEND_HASH_FOREACH_VAL(arr_hash2, data2){    

        if (Z_TYPE_P(data2) == IS_DOUBLE) {
            //total_transaction_amounts.push_back(Z_DVAL_PP(data2));
            total_transaction_amounts.push_back(Z_DVAL_P(data2));
        } 
        else if (Z_TYPE_P(data2) == IS_LONG) {
            //total_transaction_amounts.push_back(Z_DVAL_P(data2));
            total_transaction_amounts.push_back(Z_LVAL_P(data2));
        }         
        else {
            std::string temp1(Z_STRVAL_P(data2));
            double temp = strtod(temp1.c_str(),NULL);
            total_transaction_amounts.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();

    s_date_to=std::string(sc_date_to);
    
    if (sch != NULL) {
        subarray = sch->get_loan_advance_due_by_loan_ids_trasaction_amounts(loan_ids,total_transaction_amounts,s_date_to,include_expired);
    }

    add_index_zval(return_value, 0, &subarray);
}

///////////////////////////////////////////////////////////////////////////////

PHP_METHOD(Loan_scheduler, get_loan_advance_due_by_loan_ids_trasaction_principal_amounts) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    std::vector<int> loan_ids;
    std::vector<double> total_transaction_principal_amounts;
   

    zval *data1;
    HashTable *arr_hash1;
    
    zval *data2;
    HashTable *arr_hash2;

    char *sc_date_to;
    size_t sc_date_to_len;
    std::string s_date_to;
    
    zend_bool include_expired = 0;

    zval subarray;
    array_init(return_value);


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hhs|b", &arr_hash1, &arr_hash2, &sc_date_to, &sc_date_to_len,&include_expired) == FAILURE) {
        return;
    }

//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {
    ZEND_HASH_FOREACH_VAL(arr_hash1, data1) {
        if (Z_TYPE_P(data1) == IS_LONG) {
            loan_ids.push_back(Z_LVAL_P(data1));
        } else {
            std::string temp1(Z_STRVAL_P(data1));
            int temp = atoi(temp1.c_str());
            loan_ids.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();
    
//    for (zend_hash_internal_pointer_reset_ex(arr_hash2, &pointer2); zend_hash_get_current_data_ex(arr_hash2, (void**) &data2, &pointer2) == SUCCESS; zend_hash_move_forward_ex(arr_hash2, &pointer2)) {
    ZEND_HASH_FOREACH_VAL(arr_hash2, data2) {    
        if (Z_TYPE_P(data2) == IS_DOUBLE) {
            //total_transaction_amounts.push_back(Z_DVAL_P(data2));
            total_transaction_principal_amounts.push_back(Z_DVAL_P(data2));
        } 
        else if (Z_TYPE_P(data2) == IS_LONG) {
            //total_transaction_amounts.push_back(Z_DVAL_P(data2));
            total_transaction_principal_amounts.push_back(Z_LVAL_P(data2));
        }         
        else {
            std::string temp1(Z_STRVAL_P(data2));
            double temp = strtod(temp1.c_str(),NULL);
            total_transaction_principal_amounts.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();

    s_date_to=std::string(sc_date_to);
    
    if (sch != NULL) {
        subarray = sch->get_loan_advance_due_by_loan_ids_trasaction_principal_amounts(loan_ids,total_transaction_principal_amounts,s_date_to,include_expired);
    }

    add_index_zval(return_value, 0, &subarray);
}

////////////////////////////////////////////////////////////////////////////////
PHP_METHOD(Loan_scheduler, get_loan_advance_due_by_loan_ids_trasaction_amounts_between_date) {

    //std::cerr << "schedule.cc get_loan_advance_due_by_loan_ids_trasaction_amounts_between_date\n";
    
    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    std::vector<int> loan_ids;
    std::vector<double> total_transaction_amounts_before_from_date;
    std::vector<double> principle_total_transaction_amounts_before_from_date;
    std::vector<double> interest_total_transaction_amounts_before_from_date;
    std::vector<double> total_transaction_amounts_before_to_date;
    std::vector<double> principle_total_transaction_amounts_before_to_date;
    std::vector<double> interest_total_transaction_amounts_before_to_date;
   

    zval *data1;
    HashTable *arr_hash1;
    
    zval *data2;
    HashTable *arr_hash2;
    
    zval *data3;
    HashTable *arr_hash3;

    char *sc_date_from;
    size_t sc_date_from_len;
    std::string s_date_from;
    
    char *sc_date_to;
    size_t sc_date_to_len;
    std::string s_date_to;

    zval subarray;
    array_init(return_value);

    zend_bool include_expired = 0;
    
    zval *data4;
    HashTable *arr_hash4;
    
    zval *data5;
    HashTable *arr_hash5;
    
    zval *data6;
    HashTable *arr_hash6;
    
    zval *data7;
    HashTable *arr_hash7;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hhhss|bhhhh", &arr_hash1, &arr_hash2, &arr_hash3, &sc_date_from, &sc_date_from_len,&sc_date_to, &sc_date_to_len,&include_expired,&arr_hash4,&arr_hash5,&arr_hash6,&arr_hash7) == FAILURE) {
        return;
    }

    s_date_to=std::string(sc_date_to);
    s_date_from=std::string(sc_date_from);
    //std::cerr<<"\n1s_date_to "<<s_date_to<<"s_date_from "<<s_date_from<<std::endl;

//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {
    ZEND_HASH_FOREACH_VAL(arr_hash1, data1) {    

        if (Z_TYPE_P(data1) == IS_LONG) {
            loan_ids.push_back(Z_LVAL_P(data1));
        } else {
            std::string temp1(Z_STRVAL_P(data1));
            int temp = atoi(temp1.c_str());
            //std::cerr << temp <<"\n";
            loan_ids.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();
    //std::cerr<<"1\n";
//    for (zend_hash_internal_pointer_reset_ex(arr_hash2, &pointer2); zend_hash_get_current_data_ex(arr_hash2, (void**) &data2, &pointer2) == SUCCESS; zend_hash_move_forward_ex(arr_hash2, &pointer2)) {
    ZEND_HASH_FOREACH_VAL(arr_hash2, data2) {    

        if (Z_TYPE_P(data2) == IS_DOUBLE) {
            //std::cerr << "Is Double Second\n";
            total_transaction_amounts_before_from_date.push_back(Z_DVAL_P(data2));
        }
        else if (Z_TYPE_P(data2) == IS_LONG) {
            //std::cerr << "Is Double Second\n";
            total_transaction_amounts_before_from_date.push_back(Z_LVAL_P(data2));
        }
        else {
            //std::cerr << "Is String Second\n";
            std::string temp1(Z_STRVAL_P(data2));
            //std::cerr << temp1 <<"\n";
            double temp = strtod(temp1.c_str(),NULL);
            total_transaction_amounts_before_from_date.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();
    //std::cerr<<"2\n";
//    for (zend_hash_internal_pointer_reset_ex(arr_hash3, &pointer3); zend_hash_get_current_data_ex(arr_hash3, (void**) &data3, &pointer3) == SUCCESS; zend_hash_move_forward_ex(arr_hash3, &pointer3)) {
    ZEND_HASH_FOREACH_VAL(arr_hash3, data3) {    

        if (Z_TYPE_P(data3) == IS_DOUBLE) {
            //std::cerr << "Is Double Third\n";
            total_transaction_amounts_before_to_date.push_back(Z_DVAL_P(data3));
        }
        else if (Z_TYPE_P(data3) == IS_LONG) {
            //std::cerr << "Is Double Third\n";
            total_transaction_amounts_before_to_date.push_back(Z_LVAL_P(data3));
        }
        else {
            //std::cerr << "Is String Third\n";
            std::string temp1(Z_STRVAL_P(data3));
            double temp = strtod(temp1.c_str(),NULL);
            total_transaction_amounts_before_to_date.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();
    //std::cerr<<"3\n";
//    for (zend_hash_internal_pointer_reset_ex(arr_hash4, &pointer4); zend_hash_get_current_data_ex(arr_hash4, (void**) &data4, &pointer4) == SUCCESS; zend_hash_move_forward_ex(arr_hash4, &pointer4)) {
    ZEND_HASH_FOREACH_VAL(arr_hash4, data4) {    

        if (Z_TYPE_P(data4) == IS_DOUBLE) {
            //std::cerr << "Is Double Fourth\n";
            principle_total_transaction_amounts_before_from_date.push_back(Z_DVAL_P(data4));
        }
        else if (Z_TYPE_P(data4) == IS_LONG) {
            //std::cerr << "Is Double Fourth\n";
            principle_total_transaction_amounts_before_from_date.push_back(Z_LVAL_P(data4));
        }
        else {
           
            //std::cerr << "Is String Fourth\n";
            std::string temp1(Z_STRVAL_P(data4));
            //std::cerr << "Is String Fourth "<<temp1<<"\n";
            double temp = strtod(temp1.c_str(),NULL);
            principle_total_transaction_amounts_before_from_date.push_back(temp);
            //std::cerr << "Is String Fourth\n";
        }
    }ZEND_HASH_FOREACH_END();
    //std::cerr<<"4\n";
//    for (zend_hash_internal_pointer_reset_ex(arr_hash5, &pointer5); zend_hash_get_current_data_ex(arr_hash5, (void**) &data5, &pointer5) == SUCCESS; zend_hash_move_forward_ex(arr_hash5, &pointer5)) {
    ZEND_HASH_FOREACH_VAL(arr_hash5, data5) {    

        if (Z_TYPE_P(data5) == IS_DOUBLE) {
            //std::cerr << "Is Double Fifth\n";
            interest_total_transaction_amounts_before_from_date.push_back(Z_DVAL_P(data5));
        }
        else if (Z_TYPE_P(data5) == IS_LONG) {
            //std::cerr << "Is Double Fifth\n";
            interest_total_transaction_amounts_before_from_date.push_back(Z_LVAL_P(data5));
        }
        else {
            //std::cerr << "Is String Fifth\n";
            std::string temp1(Z_STRVAL_P(data5));
            double temp = strtod(temp1.c_str(),NULL);
            interest_total_transaction_amounts_before_from_date.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();
    //std::cerr<<"5\n";
//    for (zend_hash_internal_pointer_reset_ex(arr_hash6, &pointer6); zend_hash_get_current_data_ex(arr_hash6, (void**) &data6, &pointer6) == SUCCESS; zend_hash_move_forward_ex(arr_hash6, &pointer6)) {
    ZEND_HASH_FOREACH_VAL(arr_hash6, data6) {    

        if (Z_TYPE_P(data6) == IS_DOUBLE) {
             //std::cerr << "Is Double Sixth\n";
            principle_total_transaction_amounts_before_to_date.push_back(Z_DVAL_P(data6));
        }
        else if (Z_TYPE_P(data6) == IS_LONG) {
             //std::cerr << "Is Double Sixth\n";
            principle_total_transaction_amounts_before_to_date.push_back(Z_LVAL_P(data6));
        }
        else {
            //std::cerr << "Is String Sixth\n";
            std::string temp1(Z_STRVAL_P(data6));
            double temp = strtod(temp1.c_str(),NULL);
            principle_total_transaction_amounts_before_to_date.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();
    //std::cerr<<"6\n";
//    for (zend_hash_internal_pointer_reset_ex(arr_hash7, &pointer7); zend_hash_get_current_data_ex(arr_hash7, (void**) &data7, &pointer7) == SUCCESS; zend_hash_move_forward_ex(arr_hash7, &pointer7)) {
    ZEND_HASH_FOREACH_VAL(arr_hash7, data7) {    

        if (Z_TYPE_P(data7) == IS_DOUBLE) {
             //std::cerr << "Is Double Seventh\n";
            interest_total_transaction_amounts_before_to_date.push_back(Z_DVAL_P(data7));
        }
        else if (Z_TYPE_P(data7) == IS_LONG) {
             //std::cerr << "Is Double Seventh\n";
            interest_total_transaction_amounts_before_to_date.push_back(Z_LVAL_P(data7));
        }
        else {
            //std::cerr << "Is String Seventh\n";
            std::string temp1(Z_STRVAL_P(data7));
            double temp = strtod(temp1.c_str(),NULL);
            interest_total_transaction_amounts_before_to_date.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();

    //std::cerr<<"7\n";
    
    if (sch != NULL) {
        subarray = sch->get_loan_advance_due_by_loan_ids_trasaction_amounts_between_date(loan_ids,total_transaction_amounts_before_from_date,total_transaction_amounts_before_to_date,s_date_from,s_date_to,include_expired,principle_total_transaction_amounts_before_from_date,interest_total_transaction_amounts_before_from_date,principle_total_transaction_amounts_before_to_date,interest_total_transaction_amounts_before_to_date);
    }

    add_index_zval(return_value, 0, &subarray);
}
//////////

PHP_METHOD(Loan_scheduler, get_samity_wise_expired_loan_information_between_date) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    std::vector<int> samity_list;
    std::string s_date_from;
    std::string s_date_to;
    long branch_id;
    long product_id;

    zval *data1;
    HashTable *arr_hash1;

    char *sc_date_from;
    size_t sc_date_from_len;

    char *sc_date_to;
    size_t sc_date_to_len;

    zval subarray;
    //array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hssl|l", &arr_hash1, &sc_date_from, &sc_date_from_len, &sc_date_to, &sc_date_to_len, &branch_id, &product_id) == FAILURE) {
        return;
    }

    s_date_from = std::string(sc_date_from);
    s_date_to = std::string(sc_date_to);



//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {
    ZEND_HASH_FOREACH_VAL(arr_hash1, data1){    
        if (Z_TYPE_P(data1) == IS_LONG) {
            zval dup;
            ZVAL_COPY_VALUE(&dup, data1);
            convert_to_long(&dup);
            samity_list.push_back(Z_LVAL(dup));

        } else {
            std::string temp1(Z_STRVAL_P(data1));
            int temp = atoi(temp1.c_str());
            samity_list.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();

    if (sch != NULL) {
        subarray = sch->get_samity_wise_expired_loan_information_between_date(samity_list, s_date_from, s_date_to, branch_id, product_id);
    }
    //std::cerr << "After return" << std::endl;
     array_init(return_value);
//    return_value = &subarray;
//    zval_copy_ctor(return_value);
    HashTable *arr_hash;
    int array_count;
    arr_hash = Z_ARRVAL(subarray);
    array_count = zend_hash_num_elements(arr_hash);
    //std::cerr << array_count << std::endl;

    if (array_count) {
        add_index_zval(return_value, 0, &subarray);
    }
    //std::cerr << "Returned value" << std::endl;
}

//get_samity_wise_current_loan_information_before_date(std::vector<int> samity_list,std::string s_date,int branch_id,int product_id=NULL);

PHP_METHOD(Loan_scheduler, get_samity_wise_current_loan_information_before_date) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    std::vector<int> samity_list;
    std::string s_date;
    zend_long branch_id;
    zend_long product_id;
    zend_long samity_id = -1;

    zval *data1;
    HashTable *arr_hash1;

    char *sc_date;
    size_t sc_date_len;

    zval subarray;
    array_init(return_value);


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hsl|l", &arr_hash1, &sc_date, &sc_date_len, &branch_id, &product_id) == FAILURE) {
        return;
    }

    s_date = std::string(sc_date);
//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {
    ZEND_HASH_FOREACH_VAL(arr_hash1, data1){    

        if (Z_TYPE_P(data1) == IS_LONG) {
            samity_list.push_back(Z_LVAL_P(data1));
        } else {
            std::string temp1(Z_STRVAL_P(data1));
            int temp = atoi(temp1.c_str());
            samity_list.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();

    if (sch != NULL) {
        subarray = sch->get_samity_wise_current_loan_information_before_date(samity_list, s_date, branch_id, product_id);
    }

    add_index_zval(return_value, 0, &subarray);
}

PHP_METHOD(Loan_scheduler, get_samity_wise_current_loan_information_just_before_date) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    std::vector<int> samity_list;
    std::string s_date;
    zend_long branch_id = -1;
    zend_long product_id = -1;
    zend_long is_report=1;

    zval *data1;
    HashTable *arr_hash1;

    char *sc_date;
    size_t sc_date_len;

    zval subarray;
    array_init(return_value);


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hs|lll", &arr_hash1, &sc_date, &sc_date_len, &branch_id, &product_id,&is_report) == FAILURE) {
        return;
    }

    s_date = std::string(sc_date);

//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {
    ZEND_HASH_FOREACH_VAL(arr_hash1, data1){    

        if (Z_TYPE_P(data1) == IS_LONG) {
            samity_list.push_back(Z_LVAL_P(data1));
        } else {
            std::string temp1(Z_STRVAL_P(data1));
            int temp = atoi(temp1.c_str());
            samity_list.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();

    if (sch != NULL) {
        subarray = sch->get_samity_wise_current_loan_information_just_before_date(samity_list, s_date, branch_id, product_id,is_report);
    }

    add_index_zval(return_value, 0, &subarray);
}


PHP_METHOD(Loan_scheduler, get_samity_wise_current_loan_information_just_before_date_consider_fully_paid) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    std::vector<int> samity_list;
    std::string s_date;
    zend_long branch_id = -1;
    zend_long product_id = -1;
    
    zval *data1;
    HashTable *arr_hash1;

    char *sc_date;
    size_t sc_date_len;

    zval subarray;
    array_init(return_value);


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hs|ll", &arr_hash1, &sc_date, &sc_date_len, &branch_id, &product_id) == FAILURE) {
        return;
    }

    s_date = std::string(sc_date);



//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {
    ZEND_HASH_FOREACH_VAL(arr_hash1, data1){    

        if (Z_TYPE_P(data1) == IS_LONG) {
            samity_list.push_back(Z_LVAL_P(data1));
        } else {
            std::string temp1(Z_STRVAL_P(data1));
            int temp = atoi(temp1.c_str());
            samity_list.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();

    if (sch != NULL) {
        subarray = sch->get_samity_wise_current_loan_information_just_before_date_consider_fully_paid(samity_list, s_date, branch_id, product_id);
    }

    add_index_zval(return_value, 0, &subarray);
}

PHP_METHOD(Loan_scheduler, get_samity_wise_current_loan_information_between_date) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    std::vector<int> samity_list;
    std::string s_date_from;
    std::string s_date_to;
    long branch_id;
    long product_id;
    long samity_id = -1;

    zval *data1;
    HashTable *arr_hash1;

    char *sc_date_from;
    size_t sc_date_from_len;

    char *sc_date_to;
    size_t sc_date_to_len;

    zval subarray;
    array_init(return_value);


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hssl|l", &arr_hash1, &sc_date_from, &sc_date_from_len, &sc_date_to, &sc_date_to_len, &branch_id, &product_id, &samity_id) == FAILURE) {
        return;
    }

    s_date_from = std::string(sc_date_from);
    s_date_to = std::string(sc_date_to);



//    for (zend_hash_internal_pointer_reset_ex(arr_hash1, &pointer1); zend_hash_get_current_data_ex(arr_hash1, (void**) &data1, &pointer1) == SUCCESS; zend_hash_move_forward_ex(arr_hash1, &pointer1)) {
    ZEND_HASH_FOREACH_VAL(arr_hash1, data1){    
        if (Z_TYPE_P(data1) == IS_LONG) {
            samity_list.push_back(Z_LVAL_P(data1));

        } else {
            std::string temp1(Z_STRVAL_P(data1));
            int temp = atoi(temp1.c_str());
            samity_list.push_back(temp);
        }
    }ZEND_HASH_FOREACH_END();

    if (sch != NULL) {
        subarray = sch->get_samity_wise_current_loan_information_between_date(samity_list, s_date_from, s_date_to, branch_id, product_id);
    }

    add_index_zval(return_value, 0, &subarray);
}

PHP_METHOD(Loan_scheduler, get_loan_transaction_amount) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    long l_id;

    char *sc_date = NULL;
    size_t sc_date_len;

    zval subarray;
    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|s", &l_id, &sc_date, &sc_date_len) == FAILURE) {
        return;
    }
    std::string last_sch_date;
    if(sc_date)
        last_sch_date.assign(sc_date);
    if (sch != NULL) {
        subarray = sch->get_loan_transaction_amount(l_id, last_sch_date);
    }

    add_index_zval(return_value, 0, &subarray);
}

PHP_METHOD(Loan_scheduler, get_loan_transaction_amount_before_date) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    long l_id;

    char *sc_date;
    size_t sc_date_len;

    zval subarray;
    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &l_id, &sc_date, &sc_date_len) == FAILURE) {
        return;
    }
    std::string last_sch_date;
    if (sch != NULL) {
        subarray = sch->get_loan_transaction_amount_before_date(l_id, sc_date);
    }
    std::cerr << "start In Schedule.cc get_loan_transaction_amount_before_date : "<<l_id<<" sc_date "<<sc_date<<"\n";
    std::cerr << "end In Schedule.cc get_loan_transaction_amount_before_date "<<typeid(l_id).name()<<" sc_date "<<typeid(sc_date).name()<<"\n";

    add_index_zval(return_value, 0, &subarray);
    //std::cerr << " After In Schedule.cc get_loan_transaction_amount_before_date "<<l_id<<" sc_date "<<sc_date<<"\n";
}


PHP_METHOD(Loan_scheduler, get_loan_installment_information_by_loan_id_trasaction_amount) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    long l_id;

    double total_transaction_amount;

    zval subarray;
    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ld", &l_id, &total_transaction_amount) == FAILURE) {
        return;
    }
    std::string last_sch_date;
    if (sch != NULL) {
        subarray = sch->get_loan_installment_information_by_loan_id_trasaction_amount(l_id, total_transaction_amount);
    }

    add_index_zval(return_value, 0, &subarray);
}


PHP_METHOD(Loan_scheduler, get_loan_principle_interrest_distribution_by_loan_id_trasaction_amount) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    long l_id;

    double total_transaction_amount;

    zval subarray;
    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ld", &l_id, &total_transaction_amount) == FAILURE) {
        return;
    }
    std::string last_sch_date;
    if (sch != NULL) {
        subarray = sch->get_loan_principle_interrest_distribution_by_loan_id_trasaction_amount(l_id, total_transaction_amount);
    }

    add_index_zval(return_value, 0, &subarray);
}




PHP_METHOD(Loan_scheduler, get_loan_installment_information_by_loan_id_trasaction_amount_previous_transaction_amount) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    long l_id;

    double current_transaction_amount;
    double previous_total_transaction_amount=0;
    double previous_total_principle_amount=0;
    double previous_total_interest_amount=0;
    zval subarray;
    array_init(return_value);

    //std::cerr<< "In get_loan_installment_information_by_loan_id_trasaction_amount_previous_transaction_amount\n";
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ldd|dd", &l_id, &current_transaction_amount,&previous_total_transaction_amount,&previous_total_principle_amount,&previous_total_interest_amount) == FAILURE) {
        return;
    }
    std::string last_sch_date;
    if (sch != NULL) {
        //std::cerr<< "In"<< current_transaction_amount <<"##"<< previous_total_transaction_amount <<"\n";
        subarray = sch->get_loan_installment_information_by_loan_id_trasaction_amount_previous_transaction_amount(l_id, current_transaction_amount ,previous_total_transaction_amount,previous_total_principle_amount,previous_total_interest_amount);
    }

    add_index_zval(return_value, 0, &subarray);
}


PHP_METHOD(Loan_scheduler, is_valid_schedule_date) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    long l_id;

    char *sc_date;
    size_t sc_date_len;

    bool retVal;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &l_id, &sc_date, &sc_date_len) == FAILURE) {
        return;
    }
    std::string last_sch_date;
    if (sch != NULL) {
        retVal = sch->is_valid_schedule_date(l_id, sc_date);
    }

    RETURN_BOOL(retVal)
}

PHP_METHOD(Loan_scheduler, get_samity_day) {

    Loan_scheduler *sch;
//    Loan_scheduler_object *obj = (Loan_scheduler_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    Loan_scheduler_object *obj = Z_CUSTOM_OBJ_P(getThis());
    sch = obj->sch;

    long l_id;

    char *sc_date;
    size_t sc_date_len;

    std::string retVal;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &l_id, &sc_date, &sc_date_len) == FAILURE) {
        return;
    }
    std::string last_sch_date;
    if (sch != NULL) {
        retVal = sch->get_samity_day(l_id, sc_date);
    }


    RETURN_STRING(retVal.c_str())
}

const zend_function_entry sch_methods[] = {
    PHP_ME(Loan_scheduler, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Loan_scheduler, initialize, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, reinitialize, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, set_system_date, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_first_schedule_date, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_last_schedule_date, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_schedule_date_by_installment_no, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_schedule_installment_info_by_installment_no, NULL, ZEND_ACC_PUBLIC)
    

    PHP_ME(Loan_scheduler, get_loan_recoverable_amount, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_loan_recoverable_amount_branch_id, NULL, ZEND_ACC_PUBLIC)
    
    
    PHP_ME(Loan_scheduler, get_branch_wise_loan_recoverable_amount_by_product_id, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_branch_wise_loan_recoverable_amount, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_branch_wise_loan_recoverable_amount_consider_fully_paid, NULL, ZEND_ACC_PUBLIC)
    
    PHP_ME(Loan_scheduler, get_loan_schedules, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_loan_schedules_samity_wise, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_full_loan_schedules, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_loan_schedules_by_memeberlist_branch_product, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_member_wise_loan_schedules_by_branch, NULL, ZEND_ACC_PUBLIC)

    PHP_ME(Loan_scheduler, get_active_member_loan_schedule_info, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_member_wise_current_loan_information_just_before_date, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_loan_transaction_amount, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_loan_transaction_amount_before_date, NULL, ZEND_ACC_PUBLIC)
    
    PHP_ME(Loan_scheduler, get_loan_installment_information_by_loan_id_trasaction_amount_previous_transaction_amount, NULL, ZEND_ACC_PUBLIC)
    
    PHP_ME(Loan_scheduler, get_loan_installment_information_by_loan_id_trasaction_amount, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_loan_principle_interrest_distribution_by_loan_id_trasaction_amount, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_loan_principle_interrest_distribution_by_loan_ids_trasaction_amounts, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_loan_advance_due_by_loan_ids_trasaction_amounts, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_loan_advance_due_by_loan_ids_trasaction_principal_amounts, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_loan_advance_due_by_loan_ids_trasaction_amounts_between_date, NULL, ZEND_ACC_PUBLIC)
    
    PHP_ME(Loan_scheduler, get_samity_wise_expired_loan_information_before_date, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_samity_wise_expired_loan_information_between_date, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_samity_wise_current_loan_information_before_date, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_samity_wise_current_loan_information_between_date, NULL, ZEND_ACC_PUBLIC)

    PHP_ME(Loan_scheduler, get_samity_wise_current_loan_information_just_before_date, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_samity_wise_current_loan_information_just_before_date_consider_fully_paid, NULL, ZEND_ACC_PUBLIC)
    
    PHP_ME(Loan_scheduler, get_branch_wise_loan_information_before_date, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_branch_wise_loan_information_between_date, NULL, ZEND_ACC_PUBLIC)
    
    PHP_ME(Loan_scheduler, get_overdue_loans_by_branch_samity, NULL, ZEND_ACC_PUBLIC)

    PHP_ME(Loan_scheduler, is_valid_schedule_date, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Loan_scheduler, get_samity_day, NULL, ZEND_ACC_PUBLIC) 
    PHP_FE_END
};

PHP_MINIT_FUNCTION(schedule) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Loan_scheduler", sch_methods);
    sch_ce = zend_register_internal_class(&ce TSRMLS_CC);
    sch_ce->create_object = sch_create_handler;
    memcpy(&Loan_scheduler_object_handlers,
            zend_get_std_object_handlers(), sizeof (zend_object_handlers));
    Loan_scheduler_object_handlers.clone_obj = NULL;
    return SUCCESS;
}



zend_module_entry schedule_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_SCHEDULE_EXTNAME,
    NULL, /* Functions */
    PHP_MINIT(schedule), /* MINIT */
    NULL, /* MSHUTDOWN */
    NULL, /* RINIT */
    NULL, /* RSHUTDOWN */
    NULL, /* MINFO */
    PHP_SCHEDULE_EXTVER,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SCHEDULE
extern "C" {
    ZEND_GET_MODULE(schedule)
}
#endif

