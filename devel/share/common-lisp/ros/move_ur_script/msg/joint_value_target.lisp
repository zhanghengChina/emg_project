; Auto-generated. Do not edit!


(cl:in-package move_ur_script-msg)


;//! \htmlinclude joint_value_target.msg.html

(cl:defclass <joint_value_target> (roslisp-msg-protocol:ros-message)
  ((joint_value
    :reader joint_value
    :initarg :joint_value
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0)))
)

(cl:defclass joint_value_target (<joint_value_target>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <joint_value_target>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'joint_value_target)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name move_ur_script-msg:<joint_value_target> is deprecated: use move_ur_script-msg:joint_value_target instead.")))

(cl:ensure-generic-function 'joint_value-val :lambda-list '(m))
(cl:defmethod joint_value-val ((m <joint_value_target>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader move_ur_script-msg:joint_value-val is deprecated.  Use move_ur_script-msg:joint_value instead.")
  (joint_value m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <joint_value_target>) ostream)
  "Serializes a message object of type '<joint_value_target>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'joint_value))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-double-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream)))
   (cl:slot-value msg 'joint_value))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <joint_value_target>) istream)
  "Deserializes a message object of type '<joint_value_target>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'joint_value) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'joint_value)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-double-float-bits bits))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<joint_value_target>)))
  "Returns string type for a message object of type '<joint_value_target>"
  "move_ur_script/joint_value_target")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'joint_value_target)))
  "Returns string type for a message object of type 'joint_value_target"
  "move_ur_script/joint_value_target")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<joint_value_target>)))
  "Returns md5sum for a message object of type '<joint_value_target>"
  "bf4f9efdddc421a6b33ade84387cb1fe")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'joint_value_target)))
  "Returns md5sum for a message object of type 'joint_value_target"
  "bf4f9efdddc421a6b33ade84387cb1fe")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<joint_value_target>)))
  "Returns full string definition for message of type '<joint_value_target>"
  (cl:format cl:nil "float64[] joint_value~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'joint_value_target)))
  "Returns full string definition for message of type 'joint_value_target"
  (cl:format cl:nil "float64[] joint_value~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <joint_value_target>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'joint_value) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 8)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <joint_value_target>))
  "Converts a ROS message object to a list"
  (cl:list 'joint_value_target
    (cl:cons ':joint_value (joint_value msg))
))
