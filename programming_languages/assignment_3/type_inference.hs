import Data.Char
import System.Exit
import Text.Read
import Control.Exception
import System.IO.Error
import Data.Data

data Type = Int | Real | Str
    deriving (Show, Eq)

typeToStr :: Type -> String
typeToStr Int = "int"
typeToStr Real = "real"
typeToStr Str = "str"

data Token = TokType Type
           | TokLParen
           | TokRParen
           | TokLBrack
           | TokRBrack
           | TokIdent String
           | TokAnd
           | TokReturns
           | TokComma
           | TokEnd
           | TokError
    deriving (Show, Eq)

findString :: String -> String -> Bool
findString a [] | a == [] = True
                | otherwise = False
findString [] b = True
findString (a:as) (b:bs)
                | a == b = findString as bs
                | otherwise = False

tokenize :: String -> [Token]
tokenize [] = []
tokenize ('i':'n':'t':cs) = TokType Int : tokenize cs
tokenize ('s':'t':'r':cs) = TokType Str : tokenize cs
tokenize ('r':'e':'a':'l':cs) = TokType Real : tokenize cs
tokenize ('-':'>':cs) = TokReturns : tokenize cs
tokenize (c : cs)
    | c == '&'  = TokAnd    : tokenize cs
    | c == '('  = TokLParen : tokenize cs
    | c == ')'  = TokRParen : tokenize cs
    | c == '['  = TokLBrack : tokenize cs
    | c == ']'  = TokRBrack : tokenize cs
    | c == ','  = TokComma : tokenize cs
    | c == '`'  = identifier c cs
    | isSpace c = tokenize cs
    | otherwise = TokError : tokenize cs

identifier :: Char -> String -> [Token]
identifier c cs = let (name, cs') = span isAlphaNum cs in
                  TokIdent (c:name) : tokenize cs'

---- parser ----

data Tree = TypeNode {theType :: Type}
          | ListNode {theHead :: Tree, theTail :: Tree}
          | FuncParamsNode {theParamHead :: Tree, theParamTail :: Tree}
          | FuncNode {args :: Tree, returnType :: Tree}
          | VarNode {name :: String}
          | NullNode
          | ErrorNode
    deriving (Eq, Show)

getType :: Tree -> Type
getType (TypeNode t) = t

getTheHead :: Tree -> Tree
getTheHead (ListNode t _) = t

getTheTail :: Tree -> Tree
getTheTail (ListNode _ t) = t

getTheArgs :: Tree -> Tree
getTheArgs (FuncNode t _) = t

getTheParamHead :: Tree -> Tree
getTheParamHead (FuncParamsNode t _) = t

getTheParamTail :: Tree -> Tree
getTheParamTail (FuncParamsNode _ t) = t

getTheReturnType :: Tree -> Tree
getTheReturnType (FuncNode _ t) = t

isTypeNode :: Tree -> Bool
isTypeNode (TypeNode _) = True
isTypeNode (ListNode _ _) = False
isTypeNode (FuncParamsNode _ _) = False
isTypeNode (FuncNode _ _) = False
isTypeNode (VarNode _)  = False
isTypeNode (NullNode) = False

isListNode :: Tree -> Bool
isListNode (TypeNode _) = False
isListNode (ListNode _ _) = True
isListNode (FuncParamsNode _ _) = False
isListNode (FuncNode _ _) = False
isListNode (VarNode _)  = False
isListNode (NullNode) = False

isFuncParamsNode :: Tree -> Bool
isFuncParamsNode (TypeNode _) = False
isFuncParamsNode (ListNode _ _) = False
isFuncParamsNode (FuncParamsNode _ _) = True
isFuncParamsNode (FuncNode _ _) = False
isFuncParamsNode (VarNode _)  = False
isFuncParamsNode (NullNode) = False

isFuncNode :: Tree -> Bool
isFuncNode (TypeNode _) = False
isFuncNode (ListNode _ _) = False
isFuncNode (FuncParamsNode _ _) = False
isFuncNode (FuncNode _ _) = True
isFuncNode (VarNode _)  = False
isFuncNode (NullNode) = False

isVarNode :: Tree -> Bool
isVarNode (TypeNode _) = False
isVarNode (ListNode _ _) = False
isVarNode (FuncParamsNode _ _) = False
isVarNode (FuncNode _ _) = False
isVarNode (VarNode _)  = True
isVarNode (NullNode) = False

isNullNode :: Tree -> Bool
isNullNode (TypeNode _) = False
isNullNode (ListNode _ _) = False
isNullNode (FuncParamsNode _ _) = False
isNullNode (FuncNode _ _) = False
isNullNode (VarNode _)  = False
isNullNode (NullNode) = True

data ResolveType = ResolveType {
    src :: Tree,
    dst :: Tree
} deriving (Eq, Show)

getSrc :: ResolveType -> Tree
getSrc (ResolveType src _) = src

getDst :: ResolveType -> Tree
getDst (ResolveType _ dst) = dst

lookAhead :: [Token] -> Token
lookAhead [] = TokEnd
lookAhead (t:ts) = t

accept :: [Token] -> [Token]
accept [] = [TokError]
accept (t:ts) = ts

dump =  getLine `catch` eofHandler
    where
        eofHandler e = if isEOFError e then
            exitSuccess;
            else  putStrLn "ERR" >> exitSuccess

split :: Eq a => a -> [a] -> [[a]]
split d [] = []
split d s = x : split d (drop 1 y)
    where (x,y) = span (/= d) s

count :: Eq a => a -> [a] -> Int
count _ [] = 0
count c (s:sx)
    | c == s = 1 + count c sx
    | otherwise = count c sx


arglist :: [Token] -> (Tree, [Token])
arglist toks =
   let (termTree, toks') = combineTokens toks
   in
      case lookAhead toks' of
         TokComma ->
            let (exTree, toks'') = arglist (accept toks')
            in (FuncParamsNode termTree exTree, toks'')
         _ -> (FuncParamsNode termTree NullNode, toks')

tokenizeList :: [Token] -> (Tree, [Token])
tokenizeList toks =
   let (termTree, toks') = combineTokens toks
   in
      case lookAhead toks' of
         TokComma ->
            let (exTree, toks'') = arglist (accept toks')
            in (ListNode termTree exTree, toks'')
         _ -> (ListNode termTree NullNode, toks')

combineTokens :: [Token] -> (Tree, [Token])
combineTokens toks =
    case lookAhead toks of
      (TokIdent str) -> (VarNode str, accept toks)
      (TokType t) -> (TypeNode t, accept toks)
      TokLParen      ->
         let (listTree, toks') = arglist (accept toks)
         in
            if lookAhead toks' /= TokRParen
            then (ErrorNode, accept toks')
            else
                let toks'' = accept toks'
                in
                    if lookAhead toks'' /= TokReturns
                    then (ErrorNode, toks'')
                    else
                        let (typeTree, toks''') = combineTokens (accept toks'')
                        in (FuncNode listTree typeTree, toks''')
      TokLBrack      ->
         let (expTree, toks') = tokenizeList (accept toks)
         in
            if lookAhead toks' /= TokRBrack
            then (ErrorNode, accept toks')
            else (expTree, accept toks')
      _ -> (ErrorNode, accept toks)

findReplacement :: Tree -> [ResolveType] -> Tree
findReplacement t subst =
    let a = findReplacement' t subst
    in
        if isVarNode a
        then findReplacement t subst
        else
            if isNullNode a
            then NullNode
            else a

findReplacement' :: Tree -> [ResolveType] -> Tree
findReplacement' t [] = NullNode
findReplacement' t r =
    let (listHead, listTail) = (head r, tail r)
    in
        if t == getSrc listHead
        then getDst listHead
        else findReplacement' t listTail

searchTree :: Tree -> Tree -> [ResolveType] -> (Tree, [ResolveType])
searchTree (TypeNode nodeType1) (TypeNode nodeType2) subst
    | nodeType1 == nodeType2 = ((TypeNode nodeType1), subst)
    | otherwise = (ErrorNode, subst)
searchTree (TypeNode nodeType1) (ListNode tree21 tree22) subst = (ErrorNode, subst)
searchTree (TypeNode nodeType1) (FuncParamsNode paramHead paramTail) subst = (ErrorNode, subst)
searchTree (TypeNode nodeType1) (FuncNode params2 retType2) subst = (ErrorNode, subst)
searchTree (TypeNode nodeType1) (VarNode name2) subst =
    let t = findReplacement (VarNode name2) subst
    in
        if isNullNode t
        then ((TypeNode nodeType1), subst ++ [(ResolveType (VarNode name2) (TypeNode nodeType1))])
        else
            if isTypeNode t
            then
                if nodeType1 == getType t
                then (t, subst)
                else (ErrorNode, subst)
            else (ErrorNode, subst)
searchTree (TypeNode nodeType1) (NullNode) subst = (ErrorNode, subst)

searchTree (ListNode tree1 tree2) (TypeNode nodeType2) subst = (ErrorNode, subst)
searchTree (ListNode tree1 tree2) (ListNode tree21 tree22) subst =
    let (a, substa) = searchTree tree1 tree21 subst
    in
        let (b, substb) = searchTree tree2 tree22 substa
        in
            ((ListNode a b), substb)
searchTree (ListNode tree1 tree2) (VarNode name2) subst =
    let t = findReplacement (VarNode name2) subst
    in
        if isNullNode t
        then ((ListNode tree1 tree2), subst ++ [(ResolveType (VarNode name2) (ListNode tree1 tree2))])
        else
            if isListNode t
            then
                let theListHead = getTheHead t
                    theListTail = getTheTail t
                in
                    let (a, substa) = searchTree tree1 theListHead subst
                    in
                        let (b, substb) = searchTree tree2 theListTail substa
                        in
                            ((ListNode a b), substb)
            else (ErrorNode, subst)
searchTree (ListNode tree1 tree2) (NullNode) subst = (ErrorNode, subst)

searchTree (FuncParamsNode paramHead paramTail) (TypeNode nodeType2) subst = (ErrorNode, subst)
searchTree (FuncParamsNode paramHead paramTail) (ListNode tree21 tree22) subst = (ErrorNode, subst)
searchTree (FuncParamsNode paramHead paramTail) (FuncParamsNode paramHead2 paramTail2) subst =
    let (a, substa) = searchTree paramHead paramHead2 subst
    in
        let (b, substb) = searchTree paramTail paramTail2 substa
        in
            ((FuncParamsNode a b), substb)
searchTree (FuncParamsNode paramHead paramTail) (FuncNode params2 retType2) subst = (ErrorNode, subst)
searchTree (FuncParamsNode paramHead paramTail) (VarNode name2) subst =
    let t = findReplacement (VarNode name2) subst
    in
        if isNullNode t
        then ((FuncParamsNode paramHead paramTail), subst ++ [(ResolveType (VarNode name2) (FuncParamsNode paramHead paramTail))])
        else
            if isFuncParamsNode t
            then
                let theListHead = getTheParamHead t
                    theListTail = getTheParamTail t
                in
                    let (a, substa) = searchTree paramHead theListHead subst
                    in
                        let (b, substb) = searchTree paramTail theListTail substa
                        in
                            ((FuncParamsNode a b), substb)
            else (ErrorNode, subst)
searchTree (FuncParamsNode paramHead paramTail) (NullNode) subst = (NullNode, subst)

searchTree (FuncNode params retType) (TypeNode nodeType2) subst = (ErrorNode, subst)
searchTree (FuncNode params retType) (ListNode tree21 tree22) subst = (ErrorNode, subst)
searchTree (FuncNode params retType) (FuncParamsNode paramHead paramTail) subst = (ErrorNode, subst)
searchTree (FuncNode params retType) (FuncNode params2 retType2) subst =
    let (a, substa) = searchTree params params2 subst
    in
        let (b, substb) = searchTree retType retType2 substa
        in
            ((FuncNode a b), substb)
searchTree (FuncNode params retType) (VarNode name2) subst =
    let t = findReplacement (VarNode name2) subst
    in
        if isNullNode t
        then ((FuncNode params retType), subst ++ [(ResolveType (VarNode name2) (FuncNode params retType))])
        else
            if isFuncNode t
            then
                let theArgs = getTheArgs t
                    theReturnType = getTheReturnType t
                in
                    let (a, substa) = searchTree params theArgs subst
                    in
                        let (b, substb) = searchTree retType theReturnType substa
                        in
                            ((FuncNode a b), substb)
            else (ErrorNode, subst)
searchTree (FuncNode params retType) (NullNode) subst = (ErrorNode, subst)

searchTree (VarNode name) (TypeNode nodeType2) subst = searchTree (TypeNode nodeType2) (VarNode name) subst
searchTree (VarNode name) (ListNode tree21 tree22) subst = searchTree (ListNode tree21 tree22) (VarNode name) subst
searchTree (VarNode name) (FuncParamsNode paramHead paramTail) subst = searchTree (FuncParamsNode paramHead paramTail) (VarNode name) subst
searchTree (VarNode name) (FuncNode params2 retType2) subst = searchTree (FuncNode params2 retType2) (VarNode name) subst
searchTree (VarNode name) (VarNode name2) subst =
    if name == name2
    then ((VarNode name), subst)
    else
        let t = findReplacement (VarNode name2) subst
        in
            if t == NullNode
            then ((VarNode name), subst ++ [(ResolveType (VarNode name) (VarNode name2))])
            else searchTree t (VarNode name2) subst
searchTree (VarNode name) (NullNode) subst = (ErrorNode, subst)
searchTree (NullNode) (TypeNode nodeType2) subst = (ErrorNode, subst)
searchTree (NullNode) (ListNode tree21 tree22) subst = (ErrorNode, subst)
searchTree (NullNode) (FuncParamsNode paramHead paramTail) subst = (ErrorNode, subst)
searchTree (NullNode) (FuncNode params2 retType2) subst = (ErrorNode, subst)
searchTree (NullNode) (VarNode name2) subst = (ErrorNode, subst)
searchTree (NullNode) (NullNode) subst = (NullNode, subst)
searchTree _ _ subst = (ErrorNode, subst)

flattenList :: Tree -> String
flattenList (ListNode tree1 tree2) =
    let a = flattenTree tree1
        b = flattenTree tree2
    in
        if b == ""
        then a
        else a ++ ", " ++ b

flattenTree :: Tree -> String
flattenTree (TypeNode nodeType) = typeToStr nodeType
flattenTree (ListNode tree1 tree2) = "[" ++ flattenList (ListNode tree1 tree2) ++ "]"
flattenTree (FuncParamsNode paramHead paramTail) =
    let a = flattenTree paramHead
        b = flattenTree paramTail
    in
        if b == ""
        then a
        else a ++ ", " ++ b
flattenTree (FuncNode params retType) =
    let a = flattenTree params
        b = flattenTree retType
    in "(" ++ a ++ ") -> " ++ b
flattenTree (VarNode name) = name
flattenTree (NullNode) = ""

hasErrorNode :: Tree -> Bool
hasErrorNode (TypeNode nodeType) = False
hasErrorNode (ListNode tree1 tree2) = hasErrorNode tree1 || hasErrorNode tree2
hasErrorNode (FuncParamsNode paramHead paramTail) = hasErrorNode paramHead || hasErrorNode paramTail
hasErrorNode (FuncNode params retType) = hasErrorNode params || hasErrorNode retType
hasErrorNode (VarNode name) = False
hasErrorNode (NullNode) = False
hasErrorNode (ErrorNode) = True

loopInput :: [ResolveType] -> IO()
loopInput varResolves = do
    line <- dump

    if line == "QUIT" then do
        exitSuccess;
    else
        return ()

    let tokens = tokenize line

    if count TokError tokens /= 0 then do
        putStrLn "ERR"
        exitSuccess;
    else return ()

    if count TokAnd tokens /= 1 then do
        putStrLn "ERR"
        exitSuccess;
    else return ()

    let (lhs, _rhs) = span (/= TokAnd) tokens
    let rhs = drop 1 _rhs
    let (clhs, ltoks) = combineTokens lhs
    let (crhs, rtoks) = combineTokens rhs

    let ltokslen = length ltoks
    let rtokslen = length rtoks

    if ltokslen /= 0 then do
        putStrLn "ERR"
        exitSuccess;
    else return ()

    if rtokslen /= 0 then do
        putStrLn "ERR"
        exitSuccess;
    else return ()

    if hasErrorNode clhs then do
        putStrLn "ERR"
        exitSuccess;
    else return ()

    if hasErrorNode crhs then do
        putStrLn "ERR"
        exitSuccess;
    else return ()

    let (unifiedTree, newResolves) = searchTree clhs crhs varResolves

    if hasErrorNode unifiedTree then do
        putStrLn "BOTTOM"
        exitSuccess;
    else return ()

    let c = flattenTree unifiedTree
    putStrLn c

    loopInput newResolves

main = do
    loopInput []
