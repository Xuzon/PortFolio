use cine;

delimiter //
Create procedure listarDirectoresPeliculas() Begin
	 select peliculas.titulo,directores.nombre from peliculas,directores where peliculas.ID_director = directores.ID_director;
end//

Create procedure contarDirectores() Begin 
	declare cuentaVar int(5);
    declare fechaVar date;
    declare horaVar time;
    select count(ID_director) into cuentaVar from directores;
    set fechaVar = (select curdate());
    set horaVar = (select curtime());
	create table if not exists cuentaDirectores(ID int auto_increment,cuenta int(5),fecha date,hora time,primary key(ID));
    insert into cuentaDirectores values(null,cuentaVar,fechaVar,horaVar);
    select * from cuentaDirectores;
end //

Create procedure contarDirectores() Begin 
	declare cuentaVar int(5);
    declare fechaVar date;
    declare horaVar time;
	if @numeroLlamadas is null then 
		set @numeroLlamadas = 0;
    end if; 
    set @numeroLLamadas = (@numeroLLamadas +1);
    select count(ID_director) into cuentaVar from directores;
    set fechaVar = (select curdate());
    set horaVar = (select curtime());
	create table if not exists cuentaDirectores(ID int auto_increment,cuenta int(5),fecha date,hora time,primary key(ID));
    insert into cuentaDirectores values(null,cuentaVar,fechaVar,horaVar);
    select * from cuentaDirectores;
end //

delimiter ;

alter table directores add nacionalidad tinytext;
alter table peliculas add nacionalidad tinytext;
alter table actores add nacionalidad tinytext;

update Actores set nacionalidad='Aleman' where ID_actor = 1;
update Actores set nacionalidad='Ingles' where ID_actor = 2;
update Actores set nacionalidad='Americano' where ID_actor = 3;
update Actores set nacionalidad='Americano' where ID_actor = 4;
update Actores set nacionalidad='Americano' where ID_actor = 5;

update Directores set nacionalidad='Americano' where ID_director = 1;
update Directores set nacionalidad='Malayo' where ID_director = 2;
update Directores set nacionalidad='Ingles' where ID_director = 3;
update Directores set nacionalidad='Americano' where ID_director = 4;
update Directores set nacionalidad='Canadiense' where ID_director = 5;

update Peliculas set nacionalidad='Americano' where ID_pelicula = 1;
update Peliculas set nacionalidad='Americano' where ID_pelicula = 2;
update Peliculas set nacionalidad='Americano' where ID_pelicula = 3;
update Peliculas set nacionalidad='Americano' where ID_pelicula = 4;
update Peliculas set nacionalidad='Americano' where ID_pelicula = 5;

delimiter //
Create procedure consultaPorNacionalidad(IN nac tinytext) Begin 
	(select nombre,'Director' as tipo from directores where nacionalidad = nac)
    union
    (select nombre,'Actor' as tipo from actores where nacionalidad = nac)
    union
    (select titulo,'Pelicula' as tipo from peliculas where nacionalidad = nac)
    ;
end //

Create procedure peliculasPorNacionalidad(in nac tinytext, out cuenta int) Begin 
	select count(id) into cuenta from peliculas where nacionalidad = nac;
end //

Create procedure ponerEnMayusculas(INOUT aConvertir tinytext) Begin 
	set aConvertir = ucase(aConvertir);
end //

Create procedure contarDirectores() Begin  
	declare cuentaVar int(5);
    declare fechaVar date;
    declare horaVar time;
    declare tamano int;
	if @numeroLlamadas is null then  
		set @numeroLlamadas = 0;
    end if; 
    set @numeroLLamadas = (@numeroLLamadas +1);
    select count(ID_director) into cuentaVar from directores;
    set fechaVar = (select curdate());
    set horaVar = (select curtime());
	create table if not exists cuentaDirectores(ID int auto_increment,cuenta int(5),fecha date,hora time,primary key(ID));
    select count(id) into tamano from cuentaDirectores;
    if tamano = 10 then 
		delete from cuentaDirectores order by ID asc limit 1;
    end if;
    insert into cuentaDirectores values(null,cuentaVar,fechaVar,horaVar);
    select * from cuentaDirectores;
end //

#8
Create procedure extraerIMDBs(in nac varchar(255)) Begin
    DECLARE fin INT DEFAULT FALSE;
    declare imdbTemp int;
    declare nacionalidadTemp tinytext;
    DECLARE cur CURSOR FOR 
		SELECT peliculas.imdb,peliculas.nacionalidad FROM peliculas
    union SELECT actores.imdb,actores.nacionalidad FROM actores
    union SELECT directores.imdb,directores.nacionalidad FROM directores;
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET fin = TRUE;
    open cur;
    
    start transaction;
    SET @sentencia = CONCAT('drop table if exists ',nac);
	PREPARE ejecucion from @sentencia;
	EXECUTE ejecucion;
	DEALLOCATE PREPARE ejecucion;
    
	SET @sentencia = CONCAT('CREATE TABLE if not exists',nac, '(IMDB int,primary key(imdb)) ');
	PREPARE ejecucion from @sentencia;
	EXECUTE ejecucion;
	DEALLOCATE PREPARE ejecucion;
    
    bucle : loop 
		fetch cur into imdbTemp,nacionalidadTemp;
        if fin then 
			leave bucle;
		end if;
        if nacionalidadTemp = nac then 
            SET @sentencia = CONCAT('insert into ',nac, ' values(',imdbTemp,')');
			PREPARE ejecucion from @sentencia;
			EXECUTE ejecucion;
			DEALLOCATE PREPARE ejecucion;
        end if;
    end loop;
	close cur;
    commit;
end //

delimiter ;

#9
#transacciones
Set autocommit = 0;
Start transaction;
insert into directores values(null,'Hideo Kojima',0463620,52,'Japones');
commit;
Start transaction;
insert into directores values(null,'Hideo Kojama',0463621,52,'Japones');
rollback;

alter table peliculas add foreign key(ID_Director) references Directores(ID_Director); #por si acaso no tenía las referencias
#10
delimiter //
create procedure introducirPelicula(in titl tinytext,in ID_dir int,in imd int,in nac tinytext) begin 
    DECLARE fallo INT DEFAULT FALSE;
    DECLARE CONTINUE HANDLER FOR SQLEXCEPTION SET fallo = 1;#guardo si hay fallo
    set autocommit = 0;
    start transaction;
    insert into peliculas values(null,titl,ID_dir,imd,nac);#puede arrojar fallo si la ID_Director no existe
    if(fallo) then 
		rollback;
	else
		commit;
	end if;
    set autocommit = 1;
end //
delimiter ;

call introducirPelicula('Metal Gear',6,0808372,'Japones');
call introducirPelicula('Prueba fallo',8,0808377,'Japones');#va a arrojar fallo porque no hay director 8

#11
delimiter //
create trigger directores_imdb after insert on directores for each row begin
	if new.nacionalidad = "Americano" then#si el nuevo director es americano
		insert into Americano values(new.imdb);#lo meto en la tabla americano
	end if;
end//

create trigger directores_imdbUpdate after update on directores for each row begin
	if old.nacionalidad = "Americano" and  not old.nacionalidad = new.nacionalidad then#si le cambiaron la nacioanlidad y antes era americano
		delete from Americano where imdb = new.imdb;#lo borro de la tabla
	end if;
end//

create trigger directores_imdbDelete after delete on directores for each row begin
	if old.nacionalidad = "Americano" then#si borro a un americano
		delete from Americano where imdb = old.imdb;#lo borro de la tabla
	end if;
end//
insert into directores values(null,"Prueba trigger",99999,21,"Americano")//

#12
create trigger asegurarPeliculaActor before insert on peliculas for each row begin 
	if not @flagPelicula or @flagPelicula is null then #si intentas meter una pelicula sin mi procedimiento falla
		signal sqlstate '45000' set message_text = 'Intentas meter una pelicula sin actores asociados';#ya que la relacion peliculaActores necesita primero instanciar
    end if;#la pelicula para tener una ID de referencia
end//

create procedure insertPelicula(in titl tinytext,in ID_dir int,in imd int,in nac tinytext,in nom_act tinytext) begin 
	declare id_act int default false;
    declare id_pel int;
    select id_actor into id_act from actores where actores.nombre = nom_act;#guardo en id_actor el id del actor que le paso
    if (id_act) is not null then #si no es nulo
		start transaction;
		set @flagPelicula = true;#activo la flag para pasar el trigger
		insert into peliculas values(null,titl,ID_dir,imd,nac);#inserto pelicula
        select last_insert_ID() into id_pel;#cojo la ultima ID porque uso autoincremento
        insert into actorPeliculas values(id_pel,id_act);#guardo en relacion actorPeliculas
        commit;
    else #doy error directamente, podría dar fallo con el trigger si intento meterla directamente sin la flag a true
		select 'La pelicula no tiene un actor valido';
    end if;
    set @flagPelicula = false;#reactivo la flag a false para que nadie pueda insertar una pelicula
end //

delimiter ;
insert into peliculas values(null,"Prueba error",null,null,null);#va a fallar
call insertPelicula("Prueba funcional",6,88888,"Japonesa","Bruce Willis");#correcto

#ME HABIA OLVIDADO DE ON DELETE CASCADE SOY ESPECIAL
alter table actorPeliculas drop foreign key actorpeliculas_ibfk_1;
alter table actorPeliculas drop foreign key actorpeliculas_ibfk_2;
alter table actorPeliculas add foreign key (ID_Pelicula) references Peliculas(ID_Pelicula) on delete cascade;
alter table actorPeliculas add foreign key (ID_actor) references Actores(ID_actor) on delete cascade;