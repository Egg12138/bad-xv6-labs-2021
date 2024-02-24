#lang racket

(define nil null)
; demo
(define (va2pa va) va)
(define (pteindex pg_t va level) 1)
(define MAXLVL 4)
(define (pte_contents_from pa) nil)
(define (get_content pte) ( * pte))


(define (vmprint pagetable_va)
  (define (pg_iter pa level)
    (if (<= level 0)
       nil
       (for-each
        (pg_iter (get_content) (+ 1 level))
        (pte_contents_from pa))
       ))
  (pg_iter (va2pa pagetable_va) MAXLVL))
    
        