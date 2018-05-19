
(cl:in-package :asdf)

(defsystem "emg-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "IMU_sEMG" :depends-on ("_package_IMU_sEMG"))
    (:file "_package_IMU_sEMG" :depends-on ("_package"))
    (:file "imu" :depends-on ("_package_imu"))
    (:file "_package_imu" :depends-on ("_package"))
  ))