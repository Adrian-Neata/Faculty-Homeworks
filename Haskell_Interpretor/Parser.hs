module Parser
where

import Data.Maybe
import InferenceDataType
import ClassState
import Data.Map

-- Definire Program
data Program = Program [ClassState] deriving Show

initEmptyProgram :: Program
initEmptyProgram = Program (initEmptyClass:[])

getVars :: Program -> [[String]]
getVars (Program []) = []
getVars (Program ((ClassState name parent vars funcs):l))
        | (name == "Global") = getValues (ClassState name parent vars funcs) Var
        | otherwise = getVars (Program l)

getClasses :: Program -> [String]
getClasses (Program []) = []
getClasses (Program ((ClassState name _ _ _):l)) = name:(getClasses (Program l))

getParentClass :: String -> Program -> String
getParentClass s (Program []) = []
getParentClass s (Program ((ClassState name parent _ _):l))
        | (s == name) = parent
        | otherwise = getParentClass s (Program l)

getFuncsForClass :: String -> Program -> [[String]]
getFuncsForClass s (Program ((ClassState name parent vars funcs):l))
        | (name == s) = getValues (ClassState name parent vars funcs) Func
        | otherwise = getFuncsForClass s (Program l)
getFuncsForClass _ _ = []

-- Instruction poate fi ce considerați voi
data ValueType = Variable | Function | AClass deriving Show 
data Instruction = Instruction ValueType [String] | NotInstruction | Inference String Expr deriving Show

jumpSpaces :: String -> String
jumpSpaces [] = []
jumpSpaces (x:l) = if (x == ' ') then jumpSpaces l
        else (x:l)

getCurrentLine :: String -> String
getCurrentLine [] = []
getCurrentLine (x:l) = if(x == '\n') then []
        else x:(getCurrentLine l)

goToNextLine :: String -> String
goToNextLine [] = []
goToNextLine (x:l) = if(x == '\n') then l
        else goToNextLine l

getCurrentWord :: String -> String
getCurrentWord [] = []
getCurrentWord (x:l) = if ((x >= 'a' && x <= 'z') == False && (x >= 'A' && x <= 'Z') == False && (x >= '0' && x <= '9') == False) then []
        else x:(getCurrentWord l);

goToNextWord :: String -> String
goToNextWord [] = []
goToNextWord (x:l) = if((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') || (x >= '0' && x <= '9')) then goToNextWord l
        else jumpSpaces (x:l)

getCurrentArgument :: String -> String
getCurrentArgument [] = []
getCurrentArgument (x:s) = if((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z')) then getCurrentWord (x:s)
        else if(x == ' ') then getCurrentArgument (jumpSpaces s)
        else x:[]

goToNextArgument :: String -> String
goToNextArgument [] = []
goToNextArgument (x:s) = if((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z')) then goToNextWord s
        else jumpSpaces s

separate :: String -> [String]
separate [] = []
separate s = (getCurrentArgument s):(separate (goToNextArgument s))

startsWithLetter :: String -> Bool
startsWithLetter [] = False
startsWithLetter (x:s) = if ((x >= 'a' && x <= 'z') == False && (x >= 'A' && x <= 'Z') == False) then False
        else True

parseExtends :: [String] -> [String]
parseExtends [] = []
parseExtends (x:l) = if (x /= "extends") then []
--daca nu apare forma corecta extends, atunci linia e scrisa gresit
        else if (startsWithLetter (head l) && (tail l == [])) then l
--daca primul caracter din numele clasei mostenite nu e o litera sau 
--dupa clasa mostenita mai apare ceva, atunci instructiunea nu e valida
        else [] 

parseClass :: [String] -> Instruction
parseClass [] = NotInstruction
parseClass s = if (startsWithLetter (head s) == False) then NotInstruction
--daca numele clasei nu incepe cu o litera atunci nu e valid
        else if (tail s == []) then Instruction AClass ((head s):("Global"):[])
--daca nu mai sunt alte string-uri in lista atunci s-a terminat declararea cu succes
        else if (extend /= []) then Instruction AClass ((head s):extend)
--daca clasa mosteneste pe cineva atunci se apeleaza parseExtends
        else NotInstruction
        where extend = parseExtends (tail s)

parseVar :: [String] -> Instruction
parseVar [] = NotInstruction
parseVar (name:s) = if (startsWithLetter name == False) then NotInstruction
--daca numele variabilei nu incepe cu o litera atunci nu e valid
        else if (head s /= "=") then NotInstruction
--daca numele nu e urmat de = atunci instructiunea nu e valida
        else if (startsWithLetter (head (tail s)) == False) then NotInstruction
--daca numele tipului nu incepe cu o litera atunci nu e valid
        else if (tail(tail s) /= []) then NotInstruction
--daca dupa numele tipului mai este ceva atunci instructiunea nu e valida
        else Instruction Variable (name:(head(tail s)):[])

checkParam :: [String] -> Bool
checkParam (param:char:l) = if (startsWithLetter param == False) then False
--daca numele parametrului nu incepe cu o litera, atunci nu e valid
        else if(char == ")" && l == []) then True
--daca numele parametrului este urmat de ) si nu mai este nimic dupa atunci e valid
        else if(char /= ",") then False
--daca numele parametrului este urmat de altceva decat , atunci nu este valid
        else checkParam l
--se continua verificarea
checkParam _ = False

getFuncParam :: [String] -> [String]
getFuncParam [] = []
getFuncParam (param:char:l) = param:(getFuncParam l)

parseFunc :: [String] -> Instruction
parseFunc (return:clasa:a:b:name:c:s) = if (startsWithLetter return == False) then NotInstruction
--daca numele tipului returnat nu incepe cu o litera atunci nu e valid
        else if(startsWithLetter clasa == False) then NotInstruction
--daca numele clasei continatoare nu incepe cu o litera atunci nu e valid
        else if(a /= ":" || b /= ":") then NotInstruction
--daca instructiunea nu e urmata de :: atunci nu e valida
        else if(startsWithLetter name == False) then NotInstruction
--daca numele functiei nu incepe cu o litera atunci nu e valid
        else if(c /= "(") then NotInstruction
--daca dupa numele functiei nu urmeaza ( atunci instructiunea nu e valida
        else if(s /= [] && head s == ")" && tail s == []) then Instruction Function (return:clasa:name:([]))
--daca dupa ( urmeaza ) atunci functia nu primeste parametri si este valida
        else if(checkParam s == True) then Instruction Function (return:clasa:name:(getFuncParam s))
--daca primeste parametri si sunt introdusi corect atunci instructiunea e valida
        else NotInstruction
parseFunc _ = NotInstruction

removeCommas :: [String] -> [String]
removeCommas l = Prelude.filter (\x -> x /= ",") l

goToCloseParantheses :: [String] -> Integer -> [String]
goToCloseParantheses [] _ = []
goToCloseParantheses (char:l) x = if(char == "(") then goToCloseParantheses l (x+1)
        else if(char == ")" && x /= 0) then goToCloseParantheses l (x-1)
        else if(char == ")" && x == 0) then l
        else goToCloseParantheses l x

getExprFromParantheses :: [String] -> [Expr]
getExprFromParantheses (")":s) = []
getExprFromParantheses (x:s) = if(startsWithLetter x && head s /= ".") then (Va x):(getExprFromParantheses s)
        else if(head s == ".") then (getExprFromLine (x:s)):(getExprFromParantheses (goToCloseParantheses s (-1)))
        else []
getExprFromParantheses _ = []

getExprFromLine :: [String] -> Expr
getExprFromLine (varName:".":funcName:s) = FCall varName funcName (getExprFromParantheses (tail s))

parseInfer :: [String] -> Instruction
parseInfer (name:s) = if (startsWithLetter name == False) then NotInstruction
--daca numele variabilei nu incepe cu o litera atunci nu e valid
        else if (head s /= "=") then NotInstruction
--daca numele nu e urmat de = atunci instructiunea nu e valida
        else Inference name (getExprFromLine (removeCommas (tail s)))

parseLine :: String -> Instruction
parseLine line
        | getCurrentWord line == "class" = parseClass (tail (separate line))
        | getCurrentWord line == "newvar" = parseVar (tail (separate line))
        | getCurrentWord line == "infer" = parseInfer (tail (separate line))
        | otherwise = parseFunc (separate line)

parse :: String -> [Instruction]
parse [] = []
parse s = (parseLine (getCurrentLine s)) : (parse (goToNextLine s))

addVariableToClass :: [String] -> [ClassState] -> [ClassState]
addVariableToClass _ [] = []
addVariableToClass s ((ClassState name a b c):classes) = if(name == "Global") then (insertIntoClass (ClassState name a b c) Var s):classes
        else (ClassState name a b c):(addVariableToClass s classes)

addFunctionToClass :: [String] -> String -> [ClassState] -> [ClassState]
addFunctionToClass _ _ [] = []
addFunctionToClass (return:namef:params) name ((ClassState className a b c):classes) = if(name == className) then 
        (insertIntoClass (ClassState className a b c) Func  (namef:return:params)):classes
        else (ClassState className a b c):(addFunctionToClass (return:namef:params) name classes)

classExists :: String -> Program -> Bool
classExists [] _ = False
classExists _ (Program []) = False
classExists className (Program ((ClassState name _ _ _):classes)) = if(name == className) then True
        else classExists className (Program classes)

checkParams :: [String] -> Program -> Bool
checkParams [] _ = True
checkParams (className:l) prog = (classExists className prog) && (checkParams l prog)

interpret :: Instruction -> Program -> Program
interpret NotInstruction prog = prog
interpret (Instruction AClass (name:inherit:[])) (Program classes) =
        if (classExists name (Program classes) == True) then (Program classes)
--daca clasa deja exista atunci ignoram declararea
        else if (classExists inherit (Program classes) == True) then Program ((ClassState name inherit empty empty):classes)
--daca clasa mostenita exista atunci se declara normal
        else Program ((ClassState name "Global" empty empty):classes)
--daca clasa mostenita nu exista atunci o inlocuim cu Global

interpret (Instruction Variable (name:clasa:[])) (Program classes) =
        if(classExists clasa (Program classes) == False) then (Program classes)
--daca clasa dupa care a fost instantiata variabila nu exista, atunci instructiunea nu e valida
        else Program (addVariableToClass (name:clasa:[]) classes)

interpret (Instruction Function (return:clasa:name:params)) (Program classes) =
        if(classExists return (Program classes) == False) then (Program classes)
--daca tipul valorii returnate nu exista, atunci instructiunea nu e valida
        else if(classExists clasa (Program classes) == False) then (Program classes)
--daca clasa continatoare nu exista, atunci instructiunea nu e valida
        else if(checkParams params (Program classes) == False) then (Program classes)
--daca tipul unuia dintre parametri nu exista, atunci instructiunea nu e valida
        else Program (addFunctionToClass (return:name:params) clasa classes)

interpret (Inference name expr) (Program classes) = 
        case (infer expr (Program classes)) of
                Just atype -> Program (addVariableToClass (name:atype:[]) classes)
                Nothing -> (Program classes)

merge :: [a] -> [a] ->[a]
merge xs [] = xs
merge [] ys = ys
merge (x:xs) ys = x:(merge xs ys)

getFuncs :: Program -> String -> [[String]]
getFuncs (Program []) _ = []
getFuncs (Program ((ClassState name parent vars funcs):l)) className =
        if(name == className) then [parent]:(getValues (ClassState name parent vars funcs) Func)
        else getFuncs (Program l) className
--returneaza o lista de functii care apartin clasei specificate si care incepe cu parintele clasei

getAllFuncs :: Program -> String -> [[String]]
getAllFuncs (Program []) _ = []
getAllFuncs _ [] = []
getAllFuncs prog "Global" = getFuncs prog "Global"
getAllFuncs prog className = Prelude.filter (\x -> length x > 1) (merge funcs restFuncs)
        where 
                funcs = getFuncs prog className
                restFuncs = getAllFuncs prog (head (head funcs))

checkNameFuncs :: String -> [[String]] -> [[String]]
checkNameFuncs _ [] = []
checkNameFuncs funcname (func:l) = if(head func == funcname) then func:(checkNameFuncs funcname l)
        else (checkNameFuncs funcname l)

checkReturnFuncs :: String -> [[String]] -> [[String]]
checkReturnFuncs _ [] = []
checkReturnFuncs return (func:l) = if(head (tail func) == return) then func:(checkReturnFuncs return l)
        else (checkReturnFuncs return l)

checkParamFuncs :: [String] -> [[String]] -> [[String]]
checkParamFuncs _ [] = []
checkParamFuncs param (func:l) = if (param == (tail ( tail func))) then func:(checkParamFuncs param l)
        else (checkParamFuncs param l)

getVariableType :: String -> [[String]] -> String
getVariableType name [] = []
getVariableType name (pair:l) = if(head pair == name) then (head (tail pair))
        else getVariableType name l

getType :: Program -> Expr -> String
getType prog (Va varname) = getVariableType varname (getVars prog)
getType prog (FCall varname funcname list) = if(finalFunc /= []) then head (tail (head finalFunc))
        else []
        where
                finalFunc = checkParamFuncs (Prelude.map (getType prog) list) funcs
--lista de functii cu aceeasi parametri ca argumentele date
                funcs = checkNameFuncs funcname (getAllFuncs prog varType)
--lista de functii cu acelasi nume si cu aceeasi valoare de return ca si variabila
                varType = getVariableType varname (getVars prog)
--tipul variabilei care apeleaza functia

infer :: Expr -> Program -> Maybe String
infer expr prog = if (result == []) then Nothing
        else Just result
        where result = getType prog expr
