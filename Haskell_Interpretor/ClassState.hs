module ClassState
where

import Data.Map

-- Utilizat pentru a obține informații despre Variabile sau Funcții
data InstrType = Var | Func  deriving (Show, Eq)

-- TODO - Trebuie definit ClassState
data ClassState = ClassState String String (Map String [[String]]) (Map String [[String]]) deriving Show
-- nume clasa, clasa mostenita, variabile si functii
initEmptyClass :: ClassState
initEmptyClass = ClassState "Global" "Global" empty empty

getParam :: (Map String [[String]]) -> String -> [String] -> [[String]]
getParam map name list =
        case Data.Map.lookup name map of
                Just params -> list:params
                Nothing -> list:[]

insertIntoClass :: ClassState -> InstrType -> [String] -> ClassState
insertIntoClass (ClassState name parent vars funcs) Var (namevar:param) = ClassState name parent (insert namevar auxparam vars) funcs
        where auxparam = getParam vars namevar (namevar:param)
insertIntoClass (ClassState name parent vars funcs) Func (namefunc:param) = ClassState name parent vars (insert namefunc auxparam funcs)
        where auxparam = getParam funcs namefunc (namefunc:param)
getValues :: ClassState -> InstrType -> [[String]]
getValues (ClassState name parent vars funcs) Var = concat (elems vars)
getValues (ClassState name parent vars funcs) Func = concat (elems funcs)
