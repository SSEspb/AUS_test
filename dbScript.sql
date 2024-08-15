CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

CREATE USER admin WITH 
  PASSWORD '1214'
  LOGIN
  SUPERUSER
  INHERIT
  CREATEDB
  CREATEROLE;

CREATE TABLE IF NOT EXISTS objecttype
(
	id uuid NOT NULL DEFAULT uuid_generate_v4(),
    name character varying NOT NULL,
	datereg timestamp with time zone NOT NULL DEFAULT now(),
    datedel timestamp with time zone,
    CONSTRAINT objecttype_pkey PRIMARY KEY (id)
)
    WITH (
        OIDS = FALSE
    );
	
CREATE TABLE IF NOT EXISTS objects
(
	id uuid NOT NULL DEFAULT uuid_generate_v4(),
	name character varying NOT NULL,
	xcoord double precision,
	ycoord double precision,
	idobjecttype uuid NOT NULL,
	datereg timestamp with time zone NOT NULL DEFAULT now(),
    datedel timestamp with time zone,
    CONSTRAINT objects_pkey PRIMARY KEY (id),
	CONSTRAINT fk_objects_idobjecttype FOREIGN KEY (idobjecttype)
        REFERENCES objecttype (id) MATCH SIMPLE
        ON UPDATE RESTRICT ON DELETE RESTRICT
)
	WITH (
        OIDS = FALSE
    );
	
DROP VIEW IF EXISTS v_objects_name CASCADE;

CREATE OR REPLACE VIEW v_objects_name
AS
	select o.name, o.xcoord, o.ycoord, ot.name typename, o.datereg, o.datedel,
		left(o.name, 1) namestartletter, case when lower(left(o.name, 1)) ~ '[а-я]+' then true else false end isrus
	from objects o
	join objecttype ot on ot.id = o.idobjecttype
	order by namestartletter, o.name
;

DROP VIEW IF EXISTS v_objects_type CASCADE;

CREATE OR REPLACE VIEW v_objects_type
AS
	select o.name, o.xcoord, o.ycoord, ot.name typename, o.datereg, o.datedel, cntt.cnt
	from objects o
	join objecttype ot on ot.id = o.idobjecttype
	join (select idobjecttype, count(idobjecttype) cnt from objects group by idobjecttype) cntt on cntt.idobjecttype = o.idobjecttype
	order by typename, o.name
;

DROP VIEW IF EXISTS v_objects_dist CASCADE;

CREATE OR REPLACE VIEW v_objects_dist
AS
	select o.name, o.xcoord, o.ycoord, ot.name typename, o.datereg, o.datedel,
		d.dist,
		case 
			when d.dist < 100::double precision then 'До 100 ед'
			when d.dist < 1000::double precision then 'До 1000 ед'
			when d.dist < 10000::double precision then 'До 10000 ед'
		else 'Слишком далеко' end distgroup
	from objects o
	join objecttype ot on ot.id = o.idobjecttype
	join (select id, SQRT(POW(69.1 * (xcoord::float -  10::float), 2) + 
		POW(69.1 * (10::float - ycoord::float) * COS(xcoord::float / 57.3), 2)) dist from objects) d on o.id = d.id
;

DROP VIEW IF EXISTS v_objects_date CASCADE;

CREATE OR REPLACE VIEW v_objects_date
AS
	select o.name, o.xcoord, o.ycoord, ot.name typename, o.datereg, o.datedel, 
		case 
			when date_trunc('day', o.datereg) = date_trunc('day', now()) then '0 Сегодня'
			when date_trunc('day', o.datereg) = date_trunc('day', now() - INTERVAL '1 DAY') then '1 Вчера'
			when (now()::date - o.datereg::date) < 7 then '2 На этой неделе'
			when date_trunc('month', o.datereg) = date_trunc('month', now()) then '3 Ранее в этом месяце'		
			when date_trunc('year', o.datereg) = date_trunc('year', now()) then '4 В этом году'
			else '5 Ранее' 
		end dategroup
	from objects o
	join objecttype ot on ot.id = o.idobjecttype
	order by dategroup, o.datereg
;