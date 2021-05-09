#INCLUDE    <P16F84A.INC>
LIST P=16F84A

ORG 0
    BSF	    STATUS,RP0	    ;BANCO 1
    CLRF    TRISB	    ;PUERTO B SALIDA
    BSF	    TRISA,3	    ;A3 ENTRADA
    BCF	    STATUS,RP0	    ;BANCO 0 
    
    CLRF    PORTB	    ;PUERTO B EN 0
    MOVLW   D'9'	    ;VALOR DE 9
    MOVWF   PORTB	    ;CARGAR EL VALOR 9 EN EL PUERTO B
    
PROCESO:
    BTFSS   PORTA,3	    ;A3=1?
    GOTO    PROCESO	    ;NO ENTONCES VUELVE A PROCESO
    CALL    CONFIRMACION    ;SI VA A CONFIRMACION 
    CALL    DETECTAZ
    GOTO    PROCESO	    ;VUELVE A PROCESO
    
CONFIRMACION:
    BTFSC   PORTA,3	    ;A3=0?
    GOTO    CONFIRMACION    ;NO ENTONCES VUEVLE A CONFIRMACION
    CALL    DISMINUIR	    ;SI ENTONCES VA A DISMINUIR
    RETURN	
    
DISMINUIR:
    MOVLW   D'1'	    ;CARGAR EL VALOR DE RESTA 1 EN W
    SUBWF   PORTB,1	    ;RESTA W - PORTB Y GUARDA EN PORTB
    CLRW
    RETURN
    
DETECTAZ:
    MOVLW   D'255'
    SUBWF   PORTB,0
    BTFSS   STATUS,Z
    RETURN
    BTFSS   STATUS,C
    RETURN
    CLRW
    MOVLW   D'9'
    MOVWF   PORTB
    RETURN

END


    
    
    
    
    
    
    
;INCLUDE <P16F84A.INC>
;LIST P=16F84A
;ORG 0;
;
;BSF STATUS, RP0 ; Ingresando al banco 1
;CLRF TRISB ;Declarando todo el puerto B comos salida
;BSF TRISA, 3 ; Declarando el pin 3 del puerto A como entrada
;BCF STATUS, RP0 ; Regresando al Banco 0
;
;MOVLW d'9' ; Carga el valor de 9 en la variable W
;MOVWF PORTB ; Ingresa el valor de W en el puerto B.
;
;PROCESO ; Título del proceso
;BTFSS PORTA,3; ¿EL PIN 3 DEL PUERTO A ES = 1?
;GOTO PROCESO ; RESULTADO FALSO, HACE EL BUCLE
;CALL CONFIRMACION ;Resultado verdadero, llama a confirmación
;CALL PRUEBACZ ; LLama a la verificación de límite
;GOTO PROCESO; Se va al Título del proceso. Bucle
;
;CONFIRMACION ; Título de confirmación
;BTFSC PORTA, 3 ; ¿El PIN 3 del PUERTO A es = 0?
;GOTO CONFIRMACION ; RESULTADO FALSO, SIGUE EN EL BUCLE
;CALL DISMINUIR ; RESULTADO VERDADERO, LLAMA A DISMINUIR
;RETURN ; Retorna a donde lo llamó
;
;DISMINUIR ; Título para la resta
;MOVLW d'1' ; Carga el valor de "1" decimal en la variable W
;SUBWF PORTB, 1 ; Resta el valor del puerto b con la de w (PUERTO B - W), resultado lo muestra en el Puerto B
;CLRW ; Limpia la variable W
;RETURN; Retorna a donde lo llamó
;
;PRUEBACZ ; Título de verificación de límite
;MOVLW d'255' ; Carga el valor de "+127" decimal en W
;SUBWF PORTB,0 ; Resta (PUERTO B - W) y el resultado lo almacena en W
;BTFSS STATUS, Z ; ¿LA bandera Z es cero?
;RETURN ; FALSO, regresa a donde lo llamó
;BTFSS STATUS, C ; VERDADERO, ¿La Bandera C es 1?
;RETURN; FALSO, regresa a donde lo llamó
;CALL NUEVO; Verdadero, Llama a nuevo
;RETURN; Regresa a donde lo llamó
;
;NUEVO; Título de Nuevo
;MOVLW d'9'; Carga el valor de 9 decimal en W
;MOVWF PORTB; Mueve el valor de W al puerto B
;RETURN; Regresa a donde lo llamó
;
;END
