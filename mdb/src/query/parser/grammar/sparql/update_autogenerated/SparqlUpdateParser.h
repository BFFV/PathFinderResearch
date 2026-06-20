
// Generated from SparqlUpdateParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  SparqlUpdateParser : public antlr4::Parser {
public:
  enum {
    WS = 1, BASE = 2, PREFIX = 3, SELECT = 4, DISTINCT = 5, REDUCED = 6, 
    CONSTRUCT = 7, DESCRIBE = 8, ASK = 9, FROM = 10, NAMED = 11, WHERE = 12, 
    ORDER = 13, BY = 14, ASC = 15, DESC = 16, LIMIT = 17, OFFSET = 18, VALUES = 19, 
    OPTIONAL = 20, GRAPH = 21, UNION = 22, FILTER = 23, A = 24, STR = 25, 
    LANG = 26, LANGMATCHES = 27, DATATYPE = 28, BOUND = 29, SAMETERM = 30, 
    ISIRI = 31, ISURI = 32, ISBLANK = 33, ISLITERAL = 34, REGEX = 35, SUBSTR = 36, 
    K_TRUE = 37, K_FALSE = 38, LOAD = 39, CLEAR = 40, DROP = 41, ADD = 42, 
    MOVE = 43, COPY = 44, CREATE = 45, DELETE = 46, INSERT = 47, USING = 48, 
    SILENT = 49, DEFAULT = 50, ALL = 51, DATA = 52, WITH = 53, INTO = 54, 
    TO = 55, AS = 56, GROUP = 57, HAVING = 58, UNDEF = 59, BINDINGS = 60, 
    SERVICE = 61, BIND = 62, MINUS = 63, IRI = 64, URI = 65, BNODE = 66, 
    RAND = 67, ABS = 68, CEIL = 69, FLOOR = 70, ROUND = 71, CONCAT = 72, 
    STRLEN = 73, UCASE = 74, LCASE = 75, ENCODE_FOR_URI = 76, CONTAINS = 77, 
    STRSTARTS = 78, STRENDS = 79, STRBEFORE = 80, STRAFTER = 81, REPLACE = 82, 
    YEAR = 83, MONTH = 84, DAY = 85, HOURS = 86, MINUTES = 87, SECONDS = 88, 
    TIMEZONE = 89, TZ = 90, NOW = 91, UUID = 92, STRUUID = 93, MD5 = 94, 
    SHA1 = 95, SHA256 = 96, SHA384 = 97, SHA512 = 98, COALESCE = 99, IF = 100, 
    STRLANG = 101, STRDT = 102, ISNUMERIC = 103, COUNT = 104, SUM = 105, 
    MIN = 106, MAX = 107, AVG = 108, SAMPLE = 109, GROUP_CONCAT = 110, NOT = 111, 
    IN = 112, EXISTS = 113, SEPARATOR = 114, ANY = 115, SHORTEST = 116, 
    SIMPLE = 117, ACYCLIC = 118, WALKS = 119, IDENTITY = 120, INDEX = 121, 
    NFKD_CASEFOLD = 122, NORMALIZE = 123, ON = 124, TEXT = 125, TOKENIZE = 126, 
    WHITESPACE = 127, WS_KEEP_PUNCT = 128, WS_RM_PUNCT = 129, WS_SPLIT_PUNCT = 130, 
    WS_SYMBOLS = 131, HNSW = 132, CANDIDATES = 133, DIMENSION = 134, MAX_EDGES = 135, 
    METRIC = 136, IRIREF = 137, PNAME_NS = 138, PNAME_LN = 139, BLANK_NODE_LABEL = 140, 
    VAR1 = 141, VAR2 = 142, LANGTAG = 143, INTEGER = 144, DECIMAL = 145, 
    DOUBLE = 146, INTEGER_POSITIVE = 147, DECIMAL_POSITIVE = 148, DOUBLE_POSITIVE = 149, 
    INTEGER_NEGATIVE = 150, DECIMAL_NEGATIVE = 151, DOUBLE_NEGATIVE = 152, 
    STRING_LITERAL1 = 153, STRING_LITERAL2 = 154, STRING_LITERAL_LONG1 = 155, 
    STRING_LITERAL_LONG2 = 156, COMMENT = 157, REFERENCE = 158, LESS_EQUAL = 159, 
    GREATER_EQUAL = 160, NOT_EQUAL = 161, AND = 162, OR = 163, INVERSE = 164, 
    OPEN_BRACE = 165, CLOSE_BRACE = 166, OPEN_CURLY_BRACE = 167, CLOSE_CURLY_BRACE = 168, 
    OPEN_SQUARE_BRACKET = 169, CLOSE_SQUARE_BRACKET = 170, SEMICOLON = 171, 
    DOT = 172, PLUS_SIGN = 173, MINUS_SIGN = 174, ASTERISK = 175, QUESTION_MARK = 176, 
    COMMA = 177, NEGATION = 178, DIVIDE = 179, EQUAL = 180, LESS = 181, 
    GREATER = 182, PIPE = 183, ANY_OTHER = 184
  };

  enum {
    RuleQuery = 0, RulePrologue = 1, RuleBaseDecl = 2, RulePrefixDecl = 3, 
    RuleUpdate = 4, RuleInsertData = 5, RuleDeleteData = 6, RuleCreateTextIndex = 7, 
    RuleCreateHnswIndex = 8, RuleNormalizeTextIndex = 9, RuleNormalizeType = 10, 
    RuleTokenizeTextIndex = 11, RuleTokenizeType = 12, RuleQuadData = 13, 
    RuleTriplesTemplate = 14, RuleConstructTemplate = 15, RuleConstructTriples = 16, 
    RuleTriplesSameSubject = 17, RulePropertyList = 18, RulePropertyListNotEmpty = 19, 
    RuleVerb = 20, RuleObjectList = 21, RuleObject = 22, RulePropertyListPathNotEmpty = 23, 
    RulePropertyListPathNotEmptyList = 24, RuleObjectListPath = 25, RuleObjectPath = 26, 
    RuleInteger = 27, RuleTriplesNode = 28, RuleBlankNodePropertyList = 29, 
    RuleTriplesNodePath = 30, RuleBlankNodePropertyListPath = 31, RuleCollection = 32, 
    RuleCollectionPath = 33, RuleGraphNode = 34, RuleGraphNodePath = 35, 
    RuleGraphTerm = 36, RuleNil = 37, RuleRdfLiteral = 38, RuleNumericLiteral = 39, 
    RuleNumericLiteralUnsigned = 40, RuleNumericLiteralPositive = 41, RuleNumericLiteralNegative = 42, 
    RuleBooleanLiteral = 43, RuleString = 44, RuleIri = 45, RulePrefixedName = 46, 
    RuleBlankNode = 47, RuleAnon = 48
  };

  explicit SparqlUpdateParser(antlr4::TokenStream *input);

  SparqlUpdateParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~SparqlUpdateParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class QueryContext;
  class PrologueContext;
  class BaseDeclContext;
  class PrefixDeclContext;
  class UpdateContext;
  class InsertDataContext;
  class DeleteDataContext;
  class CreateTextIndexContext;
  class CreateHnswIndexContext;
  class NormalizeTextIndexContext;
  class NormalizeTypeContext;
  class TokenizeTextIndexContext;
  class TokenizeTypeContext;
  class QuadDataContext;
  class TriplesTemplateContext;
  class ConstructTemplateContext;
  class ConstructTriplesContext;
  class TriplesSameSubjectContext;
  class PropertyListContext;
  class PropertyListNotEmptyContext;
  class VerbContext;
  class ObjectListContext;
  class ObjectContext;
  class PropertyListPathNotEmptyContext;
  class PropertyListPathNotEmptyListContext;
  class ObjectListPathContext;
  class ObjectPathContext;
  class IntegerContext;
  class TriplesNodeContext;
  class BlankNodePropertyListContext;
  class TriplesNodePathContext;
  class BlankNodePropertyListPathContext;
  class CollectionContext;
  class CollectionPathContext;
  class GraphNodeContext;
  class GraphNodePathContext;
  class GraphTermContext;
  class NilContext;
  class RdfLiteralContext;
  class NumericLiteralContext;
  class NumericLiteralUnsignedContext;
  class NumericLiteralPositiveContext;
  class NumericLiteralNegativeContext;
  class BooleanLiteralContext;
  class StringContext;
  class IriContext;
  class PrefixedNameContext;
  class BlankNodeContext;
  class AnonContext; 

  class  QueryContext : public antlr4::ParserRuleContext {
  public:
    QueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PrologueContext *> prologue();
    PrologueContext* prologue(size_t i);
    std::vector<UpdateContext *> update();
    UpdateContext* update(size_t i);
    antlr4::tree::TerminalNode *EOF();
    std::vector<antlr4::tree::TerminalNode *> SEMICOLON();
    antlr4::tree::TerminalNode* SEMICOLON(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QueryContext* query();

  class  PrologueContext : public antlr4::ParserRuleContext {
  public:
    PrologueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<BaseDeclContext *> baseDecl();
    BaseDeclContext* baseDecl(size_t i);
    std::vector<PrefixDeclContext *> prefixDecl();
    PrefixDeclContext* prefixDecl(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrologueContext* prologue();

  class  BaseDeclContext : public antlr4::ParserRuleContext {
  public:
    BaseDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BASE();
    antlr4::tree::TerminalNode *IRIREF();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BaseDeclContext* baseDecl();

  class  PrefixDeclContext : public antlr4::ParserRuleContext {
  public:
    PrefixDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PREFIX();
    antlr4::tree::TerminalNode *PNAME_NS();
    antlr4::tree::TerminalNode *IRIREF();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrefixDeclContext* prefixDecl();

  class  UpdateContext : public antlr4::ParserRuleContext {
  public:
    UpdateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    InsertDataContext *insertData();
    DeleteDataContext *deleteData();
    CreateTextIndexContext *createTextIndex();
    CreateHnswIndexContext *createHnswIndex();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UpdateContext* update();

  class  InsertDataContext : public antlr4::ParserRuleContext {
  public:
    InsertDataContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INSERT();
    antlr4::tree::TerminalNode *DATA();
    QuadDataContext *quadData();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertDataContext* insertData();

  class  DeleteDataContext : public antlr4::ParserRuleContext {
  public:
    DeleteDataContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DELETE();
    antlr4::tree::TerminalNode *DATA();
    QuadDataContext *quadData();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeleteDataContext* deleteData();

  class  CreateTextIndexContext : public antlr4::ParserRuleContext {
  public:
    CreateTextIndexContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *TEXT();
    antlr4::tree::TerminalNode *INDEX();
    StringContext *string();
    antlr4::tree::TerminalNode *ON();
    IriContext *iri();
    antlr4::tree::TerminalNode *WITH();
    NormalizeTextIndexContext *normalizeTextIndex();
    TokenizeTextIndexContext *tokenizeTextIndex();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CreateTextIndexContext* createTextIndex();

  class  CreateHnswIndexContext : public antlr4::ParserRuleContext {
  public:
    CreateHnswIndexContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *HNSW();
    antlr4::tree::TerminalNode *INDEX();
    std::vector<StringContext *> string();
    StringContext* string(size_t i);
    antlr4::tree::TerminalNode *ON();
    IriContext *iri();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *DIMENSION();
    std::vector<antlr4::tree::TerminalNode *> INTEGER();
    antlr4::tree::TerminalNode* INTEGER(size_t i);
    antlr4::tree::TerminalNode *MAX_EDGES();
    antlr4::tree::TerminalNode *CANDIDATES();
    antlr4::tree::TerminalNode *METRIC();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CreateHnswIndexContext* createHnswIndex();

  class  NormalizeTextIndexContext : public antlr4::ParserRuleContext {
  public:
    NormalizeTextIndexContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NORMALIZE();
    NormalizeTypeContext *normalizeType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NormalizeTextIndexContext* normalizeTextIndex();

  class  NormalizeTypeContext : public antlr4::ParserRuleContext {
  public:
    NormalizeTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTITY();
    antlr4::tree::TerminalNode *NFKD_CASEFOLD();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NormalizeTypeContext* normalizeType();

  class  TokenizeTextIndexContext : public antlr4::ParserRuleContext {
  public:
    TokenizeTextIndexContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TOKENIZE();
    TokenizeTypeContext *tokenizeType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TokenizeTextIndexContext* tokenizeTextIndex();

  class  TokenizeTypeContext : public antlr4::ParserRuleContext {
  public:
    TokenizeTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTITY();
    antlr4::tree::TerminalNode *WS_SPLIT_PUNCT();
    antlr4::tree::TerminalNode *WS_RM_PUNCT();
    antlr4::tree::TerminalNode *WS_KEEP_PUNCT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TokenizeTypeContext* tokenizeType();

  class  QuadDataContext : public antlr4::ParserRuleContext {
  public:
    QuadDataContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_CURLY_BRACE();
    TriplesTemplateContext *triplesTemplate();
    antlr4::tree::TerminalNode *CLOSE_CURLY_BRACE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QuadDataContext* quadData();

  class  TriplesTemplateContext : public antlr4::ParserRuleContext {
  public:
    TriplesTemplateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<TriplesSameSubjectContext *> triplesSameSubject();
    TriplesSameSubjectContext* triplesSameSubject(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TriplesTemplateContext* triplesTemplate();

  class  ConstructTemplateContext : public antlr4::ParserRuleContext {
  public:
    ConstructTemplateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_CURLY_BRACE();
    antlr4::tree::TerminalNode *CLOSE_CURLY_BRACE();
    ConstructTriplesContext *constructTriples();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstructTemplateContext* constructTemplate();

  class  ConstructTriplesContext : public antlr4::ParserRuleContext {
  public:
    ConstructTriplesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TriplesSameSubjectContext *triplesSameSubject();
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);
    std::vector<ConstructTriplesContext *> constructTriples();
    ConstructTriplesContext* constructTriples(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstructTriplesContext* constructTriples();

  class  TriplesSameSubjectContext : public antlr4::ParserRuleContext {
  public:
    TriplesSameSubjectContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GraphTermContext *graphTerm();
    PropertyListNotEmptyContext *propertyListNotEmpty();
    TriplesNodeContext *triplesNode();
    PropertyListContext *propertyList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TriplesSameSubjectContext* triplesSameSubject();

  class  PropertyListContext : public antlr4::ParserRuleContext {
  public:
    PropertyListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PropertyListNotEmptyContext *propertyListNotEmpty();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyListContext* propertyList();

  class  PropertyListNotEmptyContext : public antlr4::ParserRuleContext {
  public:
    PropertyListNotEmptyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<VerbContext *> verb();
    VerbContext* verb(size_t i);
    std::vector<ObjectListContext *> objectList();
    ObjectListContext* objectList(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SEMICOLON();
    antlr4::tree::TerminalNode* SEMICOLON(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyListNotEmptyContext* propertyListNotEmpty();

  class  VerbContext : public antlr4::ParserRuleContext {
  public:
    VerbContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IriContext *iri();
    antlr4::tree::TerminalNode *A();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VerbContext* verb();

  class  ObjectListContext : public antlr4::ParserRuleContext {
  public:
    ObjectListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ObjectContext *> object();
    ObjectContext* object(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectListContext* objectList();

  class  ObjectContext : public antlr4::ParserRuleContext {
  public:
    ObjectContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GraphNodeContext *graphNode();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectContext* object();

  class  PropertyListPathNotEmptyContext : public antlr4::ParserRuleContext {
  public:
    PropertyListPathNotEmptyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VerbContext *verb();
    ObjectListPathContext *objectListPath();
    std::vector<antlr4::tree::TerminalNode *> SEMICOLON();
    antlr4::tree::TerminalNode* SEMICOLON(size_t i);
    std::vector<PropertyListPathNotEmptyListContext *> propertyListPathNotEmptyList();
    PropertyListPathNotEmptyListContext* propertyListPathNotEmptyList(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyListPathNotEmptyContext* propertyListPathNotEmpty();

  class  PropertyListPathNotEmptyListContext : public antlr4::ParserRuleContext {
  public:
    PropertyListPathNotEmptyListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VerbContext *verb();
    ObjectListContext *objectList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyListPathNotEmptyListContext* propertyListPathNotEmptyList();

  class  ObjectListPathContext : public antlr4::ParserRuleContext {
  public:
    ObjectListPathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ObjectPathContext *> objectPath();
    ObjectPathContext* objectPath(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectListPathContext* objectListPath();

  class  ObjectPathContext : public antlr4::ParserRuleContext {
  public:
    ObjectPathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GraphNodePathContext *graphNodePath();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectPathContext* objectPath();

  class  IntegerContext : public antlr4::ParserRuleContext {
  public:
    IntegerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntegerContext* integer();

  class  TriplesNodeContext : public antlr4::ParserRuleContext {
  public:
    TriplesNodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CollectionContext *collection();
    BlankNodePropertyListContext *blankNodePropertyList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TriplesNodeContext* triplesNode();

  class  BlankNodePropertyListContext : public antlr4::ParserRuleContext {
  public:
    BlankNodePropertyListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_SQUARE_BRACKET();
    PropertyListNotEmptyContext *propertyListNotEmpty();
    antlr4::tree::TerminalNode *CLOSE_SQUARE_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BlankNodePropertyListContext* blankNodePropertyList();

  class  TriplesNodePathContext : public antlr4::ParserRuleContext {
  public:
    TriplesNodePathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CollectionPathContext *collectionPath();
    BlankNodePropertyListPathContext *blankNodePropertyListPath();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TriplesNodePathContext* triplesNodePath();

  class  BlankNodePropertyListPathContext : public antlr4::ParserRuleContext {
  public:
    BlankNodePropertyListPathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_SQUARE_BRACKET();
    PropertyListPathNotEmptyContext *propertyListPathNotEmpty();
    antlr4::tree::TerminalNode *CLOSE_SQUARE_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BlankNodePropertyListPathContext* blankNodePropertyListPath();

  class  CollectionContext : public antlr4::ParserRuleContext {
  public:
    CollectionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    std::vector<GraphNodeContext *> graphNode();
    GraphNodeContext* graphNode(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CollectionContext* collection();

  class  CollectionPathContext : public antlr4::ParserRuleContext {
  public:
    CollectionPathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    std::vector<GraphNodePathContext *> graphNodePath();
    GraphNodePathContext* graphNodePath(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CollectionPathContext* collectionPath();

  class  GraphNodeContext : public antlr4::ParserRuleContext {
  public:
    GraphNodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GraphTermContext *graphTerm();
    TriplesNodeContext *triplesNode();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphNodeContext* graphNode();

  class  GraphNodePathContext : public antlr4::ParserRuleContext {
  public:
    GraphNodePathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GraphTermContext *graphTerm();
    TriplesNodePathContext *triplesNodePath();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphNodePathContext* graphNodePath();

  class  GraphTermContext : public antlr4::ParserRuleContext {
  public:
    GraphTermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IriContext *iri();
    RdfLiteralContext *rdfLiteral();
    NumericLiteralContext *numericLiteral();
    BooleanLiteralContext *booleanLiteral();
    BlankNodeContext *blankNode();
    NilContext *nil();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphTermContext* graphTerm();

  class  NilContext : public antlr4::ParserRuleContext {
  public:
    NilContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *CLOSE_BRACE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NilContext* nil();

  class  RdfLiteralContext : public antlr4::ParserRuleContext {
  public:
    RdfLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StringContext *string();
    antlr4::tree::TerminalNode *LANGTAG();
    antlr4::tree::TerminalNode *REFERENCE();
    IriContext *iri();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RdfLiteralContext* rdfLiteral();

  class  NumericLiteralContext : public antlr4::ParserRuleContext {
  public:
    NumericLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumericLiteralUnsignedContext *numericLiteralUnsigned();
    NumericLiteralPositiveContext *numericLiteralPositive();
    NumericLiteralNegativeContext *numericLiteralNegative();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumericLiteralContext* numericLiteral();

  class  NumericLiteralUnsignedContext : public antlr4::ParserRuleContext {
  public:
    NumericLiteralUnsignedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *DOUBLE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumericLiteralUnsignedContext* numericLiteralUnsigned();

  class  NumericLiteralPositiveContext : public antlr4::ParserRuleContext {
  public:
    NumericLiteralPositiveContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER_POSITIVE();
    antlr4::tree::TerminalNode *DECIMAL_POSITIVE();
    antlr4::tree::TerminalNode *DOUBLE_POSITIVE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumericLiteralPositiveContext* numericLiteralPositive();

  class  NumericLiteralNegativeContext : public antlr4::ParserRuleContext {
  public:
    NumericLiteralNegativeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER_NEGATIVE();
    antlr4::tree::TerminalNode *DECIMAL_NEGATIVE();
    antlr4::tree::TerminalNode *DOUBLE_NEGATIVE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumericLiteralNegativeContext* numericLiteralNegative();

  class  BooleanLiteralContext : public antlr4::ParserRuleContext {
  public:
    BooleanLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_TRUE();
    antlr4::tree::TerminalNode *K_FALSE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BooleanLiteralContext* booleanLiteral();

  class  StringContext : public antlr4::ParserRuleContext {
  public:
    StringContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING_LITERAL1();
    antlr4::tree::TerminalNode *STRING_LITERAL2();
    antlr4::tree::TerminalNode *STRING_LITERAL_LONG1();
    antlr4::tree::TerminalNode *STRING_LITERAL_LONG2();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StringContext* string();

  class  IriContext : public antlr4::ParserRuleContext {
  public:
    IriContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IRIREF();
    PrefixedNameContext *prefixedName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IriContext* iri();

  class  PrefixedNameContext : public antlr4::ParserRuleContext {
  public:
    PrefixedNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PNAME_LN();
    antlr4::tree::TerminalNode *PNAME_NS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrefixedNameContext* prefixedName();

  class  BlankNodeContext : public antlr4::ParserRuleContext {
  public:
    BlankNodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BLANK_NODE_LABEL();
    AnonContext *anon();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BlankNodeContext* blankNode();

  class  AnonContext : public antlr4::ParserRuleContext {
  public:
    AnonContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_SQUARE_BRACKET();
    antlr4::tree::TerminalNode *CLOSE_SQUARE_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AnonContext* anon();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

