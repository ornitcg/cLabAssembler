; file ps.as

;Entry symbol not defined
.entry LIS

;[*WARNING*][File:ps-withSecondScanErrors Line:9] Label definition before
;.entry instruction - label is ignored
;Symbol can't be both extern and entry
X: .entry W


; X is ignored
 .entry MAIN

 ;[*WARNING*][File:ps-withSecondScanErrors Line:20] Label definition before
 ;.extern instruction - label is ignored
X: .extern W

;line Ok
MAIN: .data 2,3,4

;Found undefined symbol
         mov r3,K

;Found undefined symbol
         add K, r6

 ;Found undefined symbol          (X was ignored)
          add X,MAIN

;Found undefined symbol         (X was ignored)
           add MAIN  ,X


.extern Q
