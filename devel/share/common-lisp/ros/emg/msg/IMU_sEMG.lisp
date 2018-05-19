; Auto-generated. Do not edit!


(cl:in-package emg-msg)


;//! \htmlinclude IMU_sEMG.msg.html

(cl:defclass <IMU_sEMG> (roslisp-msg-protocol:ros-message)
  ((IMU_datax
    :reader IMU_datax
    :initarg :IMU_datax
    :type cl:float
    :initform 0.0)
   (IMU_datay
    :reader IMU_datay
    :initarg :IMU_datay
    :type cl:float
    :initform 0.0)
   (IMU_dataz
    :reader IMU_dataz
    :initarg :IMU_dataz
    :type cl:float
    :initform 0.0)
   (sEMG_data
    :reader sEMG_data
    :initarg :sEMG_data
    :type cl:fixnum
    :initform 0))
)

(cl:defclass IMU_sEMG (<IMU_sEMG>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <IMU_sEMG>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'IMU_sEMG)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name emg-msg:<IMU_sEMG> is deprecated: use emg-msg:IMU_sEMG instead.")))

(cl:ensure-generic-function 'IMU_datax-val :lambda-list '(m))
(cl:defmethod IMU_datax-val ((m <IMU_sEMG>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader emg-msg:IMU_datax-val is deprecated.  Use emg-msg:IMU_datax instead.")
  (IMU_datax m))

(cl:ensure-generic-function 'IMU_datay-val :lambda-list '(m))
(cl:defmethod IMU_datay-val ((m <IMU_sEMG>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader emg-msg:IMU_datay-val is deprecated.  Use emg-msg:IMU_datay instead.")
  (IMU_datay m))

(cl:ensure-generic-function 'IMU_dataz-val :lambda-list '(m))
(cl:defmethod IMU_dataz-val ((m <IMU_sEMG>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader emg-msg:IMU_dataz-val is deprecated.  Use emg-msg:IMU_dataz instead.")
  (IMU_dataz m))

(cl:ensure-generic-function 'sEMG_data-val :lambda-list '(m))
(cl:defmethod sEMG_data-val ((m <IMU_sEMG>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader emg-msg:sEMG_data-val is deprecated.  Use emg-msg:sEMG_data instead.")
  (sEMG_data m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <IMU_sEMG>) ostream)
  "Serializes a message object of type '<IMU_sEMG>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'IMU_datax))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'IMU_datay))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'IMU_dataz))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let* ((signed (cl:slot-value msg 'sEMG_data)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <IMU_sEMG>) istream)
  "Deserializes a message object of type '<IMU_sEMG>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'IMU_datax) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'IMU_datay) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'IMU_dataz) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'sEMG_data) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<IMU_sEMG>)))
  "Returns string type for a message object of type '<IMU_sEMG>"
  "emg/IMU_sEMG")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'IMU_sEMG)))
  "Returns string type for a message object of type 'IMU_sEMG"
  "emg/IMU_sEMG")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<IMU_sEMG>)))
  "Returns md5sum for a message object of type '<IMU_sEMG>"
  "3339539c09cc8fdb8b210ad22bc8a75f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'IMU_sEMG)))
  "Returns md5sum for a message object of type 'IMU_sEMG"
  "3339539c09cc8fdb8b210ad22bc8a75f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<IMU_sEMG>)))
  "Returns full string definition for message of type '<IMU_sEMG>"
  (cl:format cl:nil "float64 IMU_datax~%float64 IMU_datay~%float64 IMU_dataz~%int8 sEMG_data~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'IMU_sEMG)))
  "Returns full string definition for message of type 'IMU_sEMG"
  (cl:format cl:nil "float64 IMU_datax~%float64 IMU_datay~%float64 IMU_dataz~%int8 sEMG_data~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <IMU_sEMG>))
  (cl:+ 0
     8
     8
     8
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <IMU_sEMG>))
  "Converts a ROS message object to a list"
  (cl:list 'IMU_sEMG
    (cl:cons ':IMU_datax (IMU_datax msg))
    (cl:cons ':IMU_datay (IMU_datay msg))
    (cl:cons ':IMU_dataz (IMU_dataz msg))
    (cl:cons ':sEMG_data (sEMG_data msg))
))
