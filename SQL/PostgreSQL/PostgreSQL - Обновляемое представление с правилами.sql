-------------------------------------------------
-- Пример создания обновляемых представлений
-- на основе view и системы правил
-------------------------------------------------
-- ТАБЛИЦА
CREATE TABLE tab
(
  num integer NOT NULL,
  "name" character varying(50),
  addr character varying(30),
  CONSTRAINT tab_pkey PRIMARY KEY (num)
)
WITH (OIDS=FALSE);
ALTER TABLE tab OWNER TO postgres;

-- ПРЕДСТАВЛЕНИЕ
-------------------------------------------------
CREATE OR REPLACE VIEW mytab AS 
 SELECT tab.num, tab.name, tab.addr
   FROM tab;
ALTER TABLE mytab OWNER TO postgres;

-- ПРАВИЛА на ПРЕДСТАВЛЕНИЕ (аналог триггеров)
-- на INSERT UPDATE DELETE
-- new - название новый (изменяемых записей)
-- old - название старых записей
-------------------------------------------------
CREATE OR REPLACE RULE rule_sel AS
    ON INSERT TO mytab DO INSTEAD   -- тип правила
INSERT INTO tab (num, name, addr)  -- тело правлила
  VALUES (new.num, new.name, new.addr);

CREATE OR REPLACE RULE test_upd AS
    ON UPDATE TO mytab DO INSTEAD  
UPDATE tab SET num = new.num, name = new.name, addr = new.addr
  WHERE tab.num = old.num;   -- условие правила (связь с таблицей по ключу)

CREATE OR REPLACE RULE test_del AS
    ON DELETE TO mytab DO INSTEAD  
DELETE FROM tab
  WHERE tab.num = old.num;

--ПРИМЕРЫ запросов 
-------------------------------------------------
insert into mytab values (1,'sally','spb')

update mytab set num=20 where num>110

delete from mytab where name='sally'