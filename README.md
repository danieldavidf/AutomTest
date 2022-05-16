# AutomTest
Towards a Test Case Generation Tool Based on Functional Requirements
https://dl.acm.org/doi/10.1145/3439961.3440002

## Requisitos:
PySimpleGUI

## Instalações:
sudo apt install python3-pip

sudo apt install python3-tk

pip3 install pysimplegui

### Para executar no Linux:
python3 main.py

## Observações:

### Observação 1:

Caso receba o erro:

/usr/lib/python3/dist-packages/requests/__init__.py:89: RequestsDependencyWarning: urllib3 (1.26.8) or chardet (3.0.4) doesn't match a supported version!
  warnings.warn("urllib3 ({}) or chardet ({}) doesn't match a supported "
Traceback (most recent call last):
  File "main.py", line 158, in \<module\>
    if ( p.telaInicialConjTesteCorreta(vals2[1],vals2[2]) and p.entradaTipoCorreta(MUT.output_type,vals2[3],vals2[4],vals2[5])):
KeyError: 1

Favor rodar o comando:

pip3 install --upgrade requests

referência: <https://stackoverflow.com/questions/56155627/requestsdependencywarning-urllib3-1-25-2-or-chardet-3-0-4-doesnt-match-a-s>