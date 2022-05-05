(set-info :smt-lib-version 2.6)
(set-logic BV)
(set-info :source |
Generated by: Mathias Preiner
Generated on: 2018-04-06
Application: Verification of invertibility conditions generated in [1].
Target solver: Boolector, CVC4, Z3
Publication:
[1] Solving Quantified Bit-Vectors using Invertibility Conditions
    Aina Niemetz, Mathias Preiner, Andrew Reynolds, Clark Barrett and Cesare Tinelli
    To appear in CAV 2018
|)
(set-info :license "https://creativecommons.org/licenses/by/4.0/")
(set-info :category "crafted")
(set-info :status unsat)
(declare-fun s () (_ BitVec 4))
(declare-fun t () (_ BitVec 4))

;(define-fun udivtotal ((a (_ BitVec 4)) (b (_ BitVec 4))) (_ BitVec 4)
;  (ite (= b (_ bv0 4)) (bvnot (_ bv0 4)) (bvudiv a b))
;)
(declare-oracle-fun udivtotal udivtotal4 ((a (_ BitVec 4)) (b (_ BitVec 4))) (_ BitVec 4))
(define-fun uremtotal ((a (_ BitVec 4)) (b (_ BitVec 4))) (_ BitVec 4)
  (ite (= b (_ bv0 4)) a (bvurem a b))
)
(define-fun min () (_ BitVec 4)
  (bvnot (bvlshr (bvnot (_ bv0 4)) (_ bv1 4)))
)
(define-fun max () (_ BitVec 4)
  (bvnot min)
)

(define-fun SC ((s (_ BitVec 4)) (t (_ BitVec 4))) Bool
(and (bvslt t (bvand s max)) (bvslt t (bvor s max)))
)

(assert
 (not
  (and
  (=> (SC s t) (exists ((x (_ BitVec 4))) (bvsgt (bvashr s x) t)))
  (=> (exists ((x (_ BitVec 4))) (bvsgt (bvashr s x) t)) (SC s t))
  )
 )
)
(check-sat)
(exit)
