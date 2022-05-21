# AutomTest

- [Paper](https://dl.acm.org/doi/10.1145/3439961.3440002) (Towards a Test Case Generation Tool Based on Functional Requirements)
- [Tool demonstration video](https://www.youtube.com/watch?v=xpNCwl_xpa0)

## Setup

  ### Requirements:

  1. ```sudo apt install python3-pip```
  2. ```sudo apt install python3-tk``` (Tkinter)
  3. ```pip3 install pysimplegui``` (PySimpleGUI)

  ### Running on linux:
  ```
  python3 main.py
  ```

## Remarks

#### 1. Specification of some variable types

  - String

    - predefined sets:

    ```
    sign/signs
    alphanumeric/alphanumerics
    any/all
    number/numbers
    letter/letters	
    ```

    - substrings (example)
    
    ```
    [all][letter][alphanumeric]
    ```

    - size (example)
    ```
    [1~4][1][2~6]
    ```

  - Date:
    ```
    YYYY/MM/DD or YYYY-MM-DD
    ```

#### 2. Possible error in package version ***requests***

If you get this error:

> /usr/lib/python3/dist-packages/requests/__init__.py:89: RequestsDependencyWarning: urllib3 (1.26.8) or chardet (3.0.4) doesn't match a supported version!
> warnings.warn("urllib3 ({}) or chardet ({}) doesn't match a supported "
> Traceback (most recent call last):  File "main.py", line 158, in \<module\>
> if ( p.telaInicialConjTesteCorreta(vals2[1],vals2[2]) and p.entradaTipoCorreta(MUT.output_type,vals2[3],vals2[4],vals2[5])): KeyError: 1

Run the command:

```
pip3 install --upgrade requests
```

Reference: <https://stackoverflow.com/questions/56155627/requestsdependencywarning-urllib3-1-25-2-or-chardet-3-0-4-doesnt-match-a-s>
