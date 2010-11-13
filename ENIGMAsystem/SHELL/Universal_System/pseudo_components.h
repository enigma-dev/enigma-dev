#define e_offsetof(local_member)(char*)&((enigma::object_locals*)&enigma::ENIGMA_global_instance)->local_member - (char*)&enigma::ENIGMA_global_instance
ptrdiff_t alarmStructOffset = e_offsetof(alarm);
