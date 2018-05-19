
(cl:in-package :asdf)

(defsystem "move_ur_script-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "joint_value_target" :depends-on ("_package_joint_value_target"))
    (:file "_package_joint_value_target" :depends-on ("_package"))
  ))