#!/bin/bash

frames=(
'522' '527' '883' '1199' '1249' '1303' '1375' '1389' '1401' '1673'
'1719' '1934' '1967' '1977' '2511' '2538' '2629' '2676' '2681' '2707'
'2862' '2908' '2955' '3040' '3074' '3092' '3101' '3150' '3152' '3209'
'3262' '3333' '3555' '3766' '3835' '3939' '3972' '3978' '4072' '4073'
'4132' '4256' '4371' '4571' '4586' '4605' '4629' '4637' '4655' '4741'
'4749' '4764' '4827' '4889' '5000' '5109' '5146' '5236' '5250' '5388'
'5416' '5586' '5652' '5940' '5971' '6135' '6185' '6217' '6276' '6496'
'6680' '6693' '6717' '6763' '7042' '7052' '7109' '7122' '7157' '7167'
'7185' '7229' '7306' '7496' '7536' '7711' '7966' '8031' '8053' '8099'
'8139' '8225' '8287' '8492' '8569' '8613' '8695' '8731' '8750' '8774')

for ((i=0;i<${#frames[@]};i++))
do
    # cp ./Images/outputs/original/frame_${frames[$i]}_.png /home/gilberto/Documentos/UFPel/TCC/Documentos_Produzidos/TCC2/teste_completo/full_seg_full_pipe
    # cp ./Images/outputs/original/frame_${frames[$i]}_.png /home/gilberto/Documentos/UFPel/TCC/Documentos_Produzidos/TCC2/teste_completo/full_seg_no_blur
    # cp ./Images/outputs/original/frame_${frames[$i]}_.png /home/gilberto/Documentos/UFPel/TCC/Documentos_Produzidos/TCC2/teste_completo/full_seg_no_pos
    # cp ./Images/outputs/original/frame_${frames[$i]}_.png /home/gilberto/Documentos/UFPel/TCC/Documentos_Produzidos/TCC2/teste_completo/full_seg_no_blur_no_pos
    # cp ./Images/outputs/original/frame_${frames[$i]}_.png /home/gilberto/Documentos/UFPel/TCC/Documentos_Produzidos/TCC2/teste_completo/reduce_seg_w100_no_blur_no_pos
done