; (define make-account
;   (let ((max-withdrawal 10)) ; let goes here for shared variables   
;     (lambda (balance)
;        (lambda (amt)
;          (begin (if (< amt max-withdrawal)
;            (set! balance (- balance amt))
;            (set! max-withdrawal (+ max-withdrawal 1)))
;          balance)))))
         
; (define mine (make-account 30))
; (define yours (make-account 60))
; (mine 4)
; (mine 40)
; (mine 10)
; (yours 10)
; (yours 50)
; (yours 6)
; (mine 3)

;; Knuth test.
(define less-than-or-equal
  (lambda (x y)
    (if (> x y) #f #t)))

(define a
  (lambda (k x1 x2 x3 x4 x5)
    (letrec ((b
              (lambda ()
                (begin
                  (set! k (- k 1))
                  (a k b x1 x2 x3 x4)))))
      (if (less-than-or-equal k 0)
          (+ (x4) (x5))
          (b)))))

(a 10 (lambda () 1) (lambda () -1)
   (lambda () -1) (lambda () 1)
   (lambda () 0))


; (let* ((f (lambda (x) (if (= 0 x) 0 (f (- x 1))))))
;   (f 3))

; (let* ((y 1) (y (+ y 2)))
;   y)

  ; (let* ((a 3) (b (+ a 1)) (c (+ b 1)))
  ; c)

; (let ((x 0) (y 1))
;   (let* ((x y) (y x))
;     (cons x y)))
