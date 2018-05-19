
(cl:in-package :asdf)

(defsystem "sinsun_robot-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "robot_state" :depends-on ("_package_robot_state"))
    (:file "_package_robot_state" :depends-on ("_package"))
  ))