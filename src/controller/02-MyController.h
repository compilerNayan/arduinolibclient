#ifndef MY_CONTROLLER
#define MY_CONTROLLER

#include "01-IMyController.h"

//@RestController
//@RequestMapping("/myUrlTee")
//@Component
class MyController final : public IMyController {
  public:

  //@GetMapping("/mysomeget2ee")
  RetDto myFunee(TestDto x) override{
    return RetDto();
  }

  //@PostMapping("/somePost2ee")
  RetDto MyPostFunnee(TestDto x) override{
    println("MyPostFunnee called");
    println(x.a.value());
    println(x.b.value());
    println(x.c.value().c_str());
    return RetDto();
  }
};

#endif