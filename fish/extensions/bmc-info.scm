;;; bmc-info.scm: Displays BMC Information
;;; authors: M.P.Anand Babu <ab@gnu.org.in> 
;;; Balamurugan <bala.a@californiadigital.com>

;;; This program is free software; you can redistribute it and/or
;;; modify it under the terms of the GNU General Public License as
;;; published by the Free Software Foundation; either version 2, or (at
;;; your option) any later version.
;;; 
;;; This program is distributed in the hope that it will be useful, but
;;; WITHOUT ANY WARRANTY; without even the implied warranty of
;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;;; General Public License for more details.
;;; 
;;; You should have received a copy of the GNU General Public License
;;; along with this program; if not, write to the Free Software
;;; Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA
;;; 
;;; bmc-info.scm should be automatically loaded thru init.scm

(use-modules (ice-9 getopt-long))

(define bmc-info-exit-status 0)

(define (bmc-info-display-usage)
  (begin 
    (display "Usage: bmc-info [-?V] [-D IPMIDRIVER] [-h IPMIHOST] [-u USERNAME]\n")
    (display "                [-p PASSWORD] [-a AUTHTYPE] [-l PRIVILEGE-LEVEL]\n")
    (display "                [--no-probing] [--driver-type=IPMIDRIVER]\n")
    (display "                [--driver-address=DRIVERADDR] [--driver-device=DEVICE]\n")
    (display "                [--hostname=IPMIHOST] [--username=USERNAME]\n")
    (display "                [--password=PASSWORD] [--auth-type=AUTHTYPE]\n")
    (display "                [--priv-level=PRIVILEGE-LEVEL] [--help] [--usage]\n")
    (display "                [--version]\n")))

(define (bmc-info-display-help)
  (begin 
    (display "Usage: bmc-info [OPTION...] \n")
    (display "bmc-info displays information about BMC.\n")
    (display "\n")
    (display "      --no-probing           Do not probe IPMI devices.\n")
    (display "  -D, --driver-type=IPMIDRIVER   Use this IPMIDRIVER instead of auto selection.\n")
    (display "                              Allowed values are KCS, SMIC, SSIF and LAN.\n")
    (display "      --driver-address=DRIVERADDR\n")
    (display "                             Use this DRIVERADDR address instead of probed\n")
    (display "                             one.\n")
    (display "      --driver-device=DEVICE Use this DEVICE for IPMI driver\n")
    (display "  -h, --host=IPMIHOST        Connect to IPMIHOST.\n")
    (display "  -u, --username=USERNAME    Use USERNAME instead of NULL.  Maximum USERNAME\n")
    (display "                             length is 16.\n")
    (display "  -p, --password=PASSWORD    Use PASSWORD instead of NULL.  Maximum PASSWORD\n")
    (display "                             length is 16.\n")
    (display "  -a, --auth-type=AUTHTYPE   Use AUTHTYPE instead of NONE.  Allowed values are\n")
    (display "                             NONE, MD2, MD5, PLAIN and OEM.\n")
    (display "  -l, --priv-level=PRIVILEGE-LEVEL\n")
    (display "                             Use this PRIVILEGE-LEVEL instead of USER.  Allowed\n")
    (display "                             values are CALLBACK, USER, OPERATOR, ADMIN and\n")
    (display "                             OEM.\n")
    (display "  -?, --help                 Give this help list.\n")
    (display "      --usage                Give a short usage message.\n")
    (display "  -V, --version              Print program version.\n")
    (display "\n")
    (display "Mandatory or optional arguments to long options are also mandatory or optional\n")
    (display "for any corresponding short options.\n")
    (display "\n")
    (display "Report bugs to <freeipmi-devel@gnu.org>.\n")))

(define (bmc-info-display-version)
  (begin 
    (display (string-append "BMC Information [bmc-info-" (fi-version) "]\n"))
    (display "Copyright (C) 2003-2005 FreeIPMI Core Team\n")
    (display "This program is free software; you may redistribute it under the terms of\n")
    (display "the GNU General Public License.  This program has absolutely no warranty.\n")))

(define (bmc-info-argp args)
  (catch 'misc-error 
	 (lambda () 
	   (let* ((bmc-info-cmd-args '())
		  (option-spec '((no-probing     (single-char #\202) (value #f))
				 (driver-type    (single-char #\D)   (value #t))
				 (driver-address (single-char #\203) (value #t))
				 (driver-device  (single-char #\204) (value #t))
				 (host           (single-char #\h)   (value #t))
				 (username       (single-char #\u)   (value #t))
				 (password       (single-char #\p)   (value #t))
				 (auth-type      (single-char #\a)   (value #t))
				 (priv-level     (single-char #\l)   (value #t))
				 (help           (single-char #\?)   (value #f))
				 (usage          (single-char #\377) (value #f))
				 (version        (single-char #\V)   (value #f))))
		  (options (getopt-long args option-spec))
		  (no-probing     (option-ref options 'no-probing     #f))
		  (driver-type    (option-ref options 'driver-type    #f))
		  (driver-address (option-ref options 'driver-address #f))
		  (driver-device  (option-ref options 'driver-device  #f))
		  (host           (option-ref options 'host           #f))
		  (username       (option-ref options 'username       #f))
		  (password       (option-ref options 'password       #f))
		  (auth-type      (option-ref options 'auth-type      #f))
		  (priv-level     (option-ref options 'priv-level     #f))
		  (help-wanted    (option-ref options 'help           #f))
		  (usage-wanted   (option-ref options 'usage          #f))
		  (version-wanted (option-ref options 'version        #f))
		  (extra-args     (option-ref options '()             #f)))
	     ;; extra arguments
	     (if (and (not (null? extra-args)) (list? bmc-info-cmd-args))
		 (begin 
		   (display "Usage: bmc-info [OPTION...] \n"
			    (current-error-port))
		   (display "Try `bmc-info --help' or `bmc-info --usage' for more information.\n"
			    (current-error-port))
		   (set! bmc-info-exit-status 64)
		   (set! bmc-info-cmd-args #f)))
	     ;; --no-probing (0)
	     (if (list? bmc-info-cmd-args)
		 (set! bmc-info-cmd-args (append bmc-info-cmd-args 
						 (list no-probing))))
	     ;; --driver-type (1)
	     (if (and (string? driver-type) (list? bmc-info-cmd-args))
		 (cond 
		  ((string-ci=? driver-type "lan")
		   (set! driver-type 1))
		  ((string-ci=? driver-type "kcs")
		   (set! driver-type 2))
		  ((string-ci=? driver-type "smic")
		   (set! driver-type 3))
		  ((string-ci=? driver-type "bt")
		   (set! driver-type 4))
		  ((string-ci=? driver-type "ssif")
		   (set! driver-type 5))
		  (else 
		   (begin 
		     (display "Usage: bmc-info [OPTION...] \n"
			      (current-error-port))
		     (display "Try `bmc-info --help' or `bmc-info --usage' for more information.\n"
			      (current-error-port))
		     (set! bmc-info-exit-status 64)
		     (set! bmc-info-cmd-args #f))))
		 (set! driver-type 0))
	     (if (list? bmc-info-cmd-args)
		 (set! bmc-info-cmd-args (append bmc-info-cmd-args 
						 (list driver-type))))
	     ;; driver-address (2)
	     (if (and (string? driver-address) (list? bmc-info-cmd-args))
		 (begin 
		   (set! driver-address (string->number driver-address))
		   (if (boolean? driver-address)
		       (begin 
			 (display "Usage: bmc-info [OPTION...] \n"
				  (current-error-port))
			 (display "Try `bmc-info --help' or `bmc-info --usage' for more information.\n"
				  (current-error-port))
			 (set! bmc-info-exit-status 64)
			 (set! bmc-info-cmd-args #f)))))
	     (if (list? bmc-info-cmd-args)
		 (set! bmc-info-cmd-args (append bmc-info-cmd-args 
						 (list driver-address))))
	     ;; --driver-device (3)
	     (if (list? bmc-info-cmd-args)
		 (set! bmc-info-cmd-args (append bmc-info-cmd-args 
						 (list driver-device))))
	     ;; --host (4)
	     (if (list? bmc-info-cmd-args)
		 (set! bmc-info-cmd-args (append bmc-info-cmd-args 
						 (list host))))
	     ;; --username (5)
	     (if (and (string? username) (list? bmc-info-cmd-args))
		 (begin 
		   (if (> (string-length username) 16)
		       (begin 
			 (display "Usage: bmc-info [OPTION...] \n"
				  (current-error-port))
			 (display "Try `bmc-info --help' or `bmc-info --usage' for more information.\n"
				  (current-error-port))
			 (set! bmc-info-exit-status 64)
			 (set! bmc-info-cmd-args #f)))))
	     (if (list? bmc-info-cmd-args)
		 (set! bmc-info-cmd-args (append bmc-info-cmd-args 
						 (list username))))
	     ;; --password (6)
	     (if (and (string? password) (list? bmc-info-cmd-args))
		 (begin 
		   (if (> (string-length password) 16)
		       (begin 
			 (display "Usage: bmc-info [OPTION...] \n"
				  (current-error-port))
			 (display "Try `bmc-info --help' or `bmc-info --usage' for more information.\n"
				  (current-error-port))
			 (set! bmc-info-exit-status 64)
			 (set! bmc-info-cmd-args #f)))))
	     (if (list? bmc-info-cmd-args)
		 (set! bmc-info-cmd-args (append bmc-info-cmd-args 
						 (list password))))
	     ;; --auth-type (7)
	     (if (and (string? auth-type) (list? bmc-info-cmd-args))
		 (cond 
		  ((string-ci=? auth-type "none")
		   (set! auth-type 0))
		  ((string-ci=? auth-type "md2")
		   (set! auth-type 1))
		  ((string-ci=? auth-type "md5")
		   (set! auth-type 2))
		  ((string-ci=? auth-type "plain")
		   (set! auth-type 4))
		  ((string-ci=? auth-type "oem")
		   (set! auth-type 5))
		  (else 
		   (begin 
		     (display "Usage: bmc-info [OPTION...] \n"
			      (current-error-port))
		     (display "Try `bmc-info --help' or `bmc-info --usage' for more information.\n"
			      (current-error-port))
		     (set! bmc-info-exit-status 64)
		     (set! bmc-info-cmd-args #f))))
		 (set! auth-type 0))
	     (if (list? bmc-info-cmd-args)
		 (set! bmc-info-cmd-args (append bmc-info-cmd-args 
						 (list auth-type))))
	     ;; --priv-level (8)
	     (if (and (string? priv-level) (list? bmc-info-cmd-args))
		 (cond 
		  ((string-ci=? priv-level "callback")
		   (set! priv-level 1))
		  ((string-ci=? priv-level "user")
		   (set! priv-level 2))
		  ((string-ci=? priv-level "operator")
		   (set! priv-level 3))
		  ((string-ci=? priv-level "admin")
		   (set! priv-level 4))
		  ((string-ci=? priv-level "oem")
		   (set! priv-level 5))
		  (else 
		   (begin 
		     (display "Usage: bmc-info [OPTION...] \n"
			      (current-error-port))
		     (display "Try `bmc-info --help' or `bmc-info --usage' for more information.\n"
			      (current-error-port))
		     (set! bmc-info-exit-status 64)
		     (set! bmc-info-cmd-args #f))))
		 (set! priv-level 2))
	     (if (list? bmc-info-cmd-args)
		 (set! bmc-info-cmd-args (append bmc-info-cmd-args 
						 (list priv-level))))
	     ;; --help (9)
	     (if (list? bmc-info-cmd-args)
		 (set! bmc-info-cmd-args (append bmc-info-cmd-args 
						 (list help-wanted))))
	     ;; --usage (10)
	     (if (list? bmc-info-cmd-args)
		 (set! bmc-info-cmd-args (append bmc-info-cmd-args 
						 (list usage-wanted))))
	     ;; --version (11)
	     (if (list? bmc-info-cmd-args)
		 (set! bmc-info-cmd-args (append bmc-info-cmd-args 
						 (list version-wanted))))
	     bmc-info-cmd-args))
	 (lambda (k args . opts)
	   (display "bmc-info: error: " (current-error-port))
	   (display (cadr opts) (current-error-port))
	   (display "\n" (current-error-port))
	   (display "Usage: bmc-info [OPTION...] \n"
		    (current-error-port))
	   (display "Try `bmc-info --help' or `bmc-info --usage' for more information.\n"
		    (current-error-port))
	   (set! bmc-info-exit-status 64)
	   #f)))

(define (bmc-info-get-help-option cmd-args)
  (list-ref cmd-args 9))

(define (bmc-info-get-usage-option cmd-args)
  (list-ref cmd-args 10))

(define (bmc-info-get-version-option cmd-args)
  (list-ref cmd-args 11))

(define (bmc-info-main cmd-args)
  (cond 
   ((bmc-info-get-help-option cmd-args)
    (bmc-info-display-help))
   ((bmc-info-get-usage-option cmd-args)
    (bmc-info-display-usage))
   ((bmc-info-get-version-option cmd-args)
    (bmc-info-display-version))
   (else 
    (and (fi-ipmi-open cmd-args)
	 (begin (fi-cmd-get-dev-id-display)
		(fi-ipmi-close))))))

(define (bmc-info args)
  "fish bmc-info main"
  (let ((cmd-args (bmc-info-argp (append (list "bmc-info") 
					 (list->strlist args)))))
    (if (list? cmd-args)
	(bmc-info-main cmd-args))))

(fi-register-command! 
 (list "bmc-info" 
       (string-append 
	"bmc-info [--no-probing] [--driver-type=IPMIDRIVER]\n"
	"         [--driver-address=DRIVERADDR] [--driver-device=DEVICE]\n"
	"         [--hostname=IPMIHOST] [--username=USERNAME]\n"
	"         [--password=PASSWORD] [--auth-type=AUTHTYPE]\n"
	"         [--priv-level=PRIVILEGE-LEVEL] [--help] [--usage] [--version]\n"
	"\n"
	"   Displays information about BMC.")))

