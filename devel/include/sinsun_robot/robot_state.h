// Generated by gencpp from file sinsun_robot/robot_state.msg
// DO NOT EDIT!


#ifndef SINSUN_ROBOT_MESSAGE_ROBOT_STATE_H
#define SINSUN_ROBOT_MESSAGE_ROBOT_STATE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace sinsun_robot
{
template <class ContainerAllocator>
struct robot_state_
{
  typedef robot_state_<ContainerAllocator> Type;

  robot_state_()
    : currentFlangePostion()
    , currentFlangePostionRight()
    , axisCurrentAngle()
    , axisCurrentAngleRight()  {
    }
  robot_state_(const ContainerAllocator& _alloc)
    : currentFlangePostion(_alloc)
    , currentFlangePostionRight(_alloc)
    , axisCurrentAngle(_alloc)
    , axisCurrentAngleRight(_alloc)  {
  (void)_alloc;
    }



   typedef std::vector<double, typename ContainerAllocator::template rebind<double>::other >  _currentFlangePostion_type;
  _currentFlangePostion_type currentFlangePostion;

   typedef std::vector<double, typename ContainerAllocator::template rebind<double>::other >  _currentFlangePostionRight_type;
  _currentFlangePostionRight_type currentFlangePostionRight;

   typedef std::vector<double, typename ContainerAllocator::template rebind<double>::other >  _axisCurrentAngle_type;
  _axisCurrentAngle_type axisCurrentAngle;

   typedef std::vector<double, typename ContainerAllocator::template rebind<double>::other >  _axisCurrentAngleRight_type;
  _axisCurrentAngleRight_type axisCurrentAngleRight;




  typedef boost::shared_ptr< ::sinsun_robot::robot_state_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::sinsun_robot::robot_state_<ContainerAllocator> const> ConstPtr;

}; // struct robot_state_

typedef ::sinsun_robot::robot_state_<std::allocator<void> > robot_state;

typedef boost::shared_ptr< ::sinsun_robot::robot_state > robot_statePtr;
typedef boost::shared_ptr< ::sinsun_robot::robot_state const> robot_stateConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::sinsun_robot::robot_state_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::sinsun_robot::robot_state_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace sinsun_robot

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg'], 'sinsun_robot': ['/home/leon/emg_project/src/sinsun_robot/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::sinsun_robot::robot_state_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::sinsun_robot::robot_state_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::sinsun_robot::robot_state_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::sinsun_robot::robot_state_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::sinsun_robot::robot_state_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::sinsun_robot::robot_state_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::sinsun_robot::robot_state_<ContainerAllocator> >
{
  static const char* value()
  {
    return "3dcf316e4e53981483352b964db6e5f9";
  }

  static const char* value(const ::sinsun_robot::robot_state_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x3dcf316e4e539814ULL;
  static const uint64_t static_value2 = 0x83352b964db6e5f9ULL;
};

template<class ContainerAllocator>
struct DataType< ::sinsun_robot::robot_state_<ContainerAllocator> >
{
  static const char* value()
  {
    return "sinsun_robot/robot_state";
  }

  static const char* value(const ::sinsun_robot::robot_state_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::sinsun_robot::robot_state_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float64[] currentFlangePostion\n\
float64[] currentFlangePostionRight\n\
float64[] axisCurrentAngle\n\
float64[] axisCurrentAngleRight\n\
";
  }

  static const char* value(const ::sinsun_robot::robot_state_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::sinsun_robot::robot_state_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.currentFlangePostion);
      stream.next(m.currentFlangePostionRight);
      stream.next(m.axisCurrentAngle);
      stream.next(m.axisCurrentAngleRight);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct robot_state_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::sinsun_robot::robot_state_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::sinsun_robot::robot_state_<ContainerAllocator>& v)
  {
    s << indent << "currentFlangePostion[]" << std::endl;
    for (size_t i = 0; i < v.currentFlangePostion.size(); ++i)
    {
      s << indent << "  currentFlangePostion[" << i << "]: ";
      Printer<double>::stream(s, indent + "  ", v.currentFlangePostion[i]);
    }
    s << indent << "currentFlangePostionRight[]" << std::endl;
    for (size_t i = 0; i < v.currentFlangePostionRight.size(); ++i)
    {
      s << indent << "  currentFlangePostionRight[" << i << "]: ";
      Printer<double>::stream(s, indent + "  ", v.currentFlangePostionRight[i]);
    }
    s << indent << "axisCurrentAngle[]" << std::endl;
    for (size_t i = 0; i < v.axisCurrentAngle.size(); ++i)
    {
      s << indent << "  axisCurrentAngle[" << i << "]: ";
      Printer<double>::stream(s, indent + "  ", v.axisCurrentAngle[i]);
    }
    s << indent << "axisCurrentAngleRight[]" << std::endl;
    for (size_t i = 0; i < v.axisCurrentAngleRight.size(); ++i)
    {
      s << indent << "  axisCurrentAngleRight[" << i << "]: ";
      Printer<double>::stream(s, indent + "  ", v.axisCurrentAngleRight[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // SINSUN_ROBOT_MESSAGE_ROBOT_STATE_H
