package storagesystem.structure;

import static org.assertj.core.api.Assertions.assertThat;
import static storagesystem.structure.Catalog.*;
import static storagesystem.structure.Shoes.*;

import java.util.Collection;

import static storagesystem.price.strategy.IndustrialShoesStrategy.*;
import static storagesystem.price.strategy.PrivateEntityShoesStrategy.*;

import org.junit.Before;
import org.junit.Test;

import storagesystem.price.strategy.TypeOfManufactureStrategy;



public class CatalogTesting {
	
	private Catalog catalog;
	
	@Before
	public void init() {
		catalog = createCatalog("Catalog");
	}
	
	@Test
	public void testAddContents() {
		TypeOfManufactureStrategy industrialType = industrialWith(3, 5.0);
		ShoeStorageSystem airForce1 = createShoesWithoutDescription("AirForce1", 40.0, industrialType);
		Collection<ShoeStorageSystem> contents = catalog.getContents();
		
		catalog.addContents(airForce1);
		
		assertThat(contents.size()).isEqualTo(1);
		assertThat(contents.contains(airForce1)).isTrue();
	}
	
	@Test
	public void testRemoveContents() {
		TypeOfManufactureStrategy industrialType = industrialWith(3, 5.0);
		TypeOfManufactureStrategy privateType = privateEntityWith(6, 10);
		ShoeStorageSystem airForce1 = createShoesWithoutDescription("AirForce1", 40.0, industrialType);
		ShoeStorageSystem derimond = createShoesWithoutDescription("Derimond", 50, privateType);
		Collection<ShoeStorageSystem> contents = catalog.getContents();
		
		catalog.addContents(airForce1);
		catalog.addContents(derimond);
		catalog.removeContents(airForce1);
		
		assertThat(contents.size()).isEqualTo(1);
		assertThat(contents.contains(derimond)).isTrue();
		assertThat(contents.contains(airForce1)).isFalse();
	}
	
	@Test
	public void testGetInfoWhenIsEmpty() {
		assertThat(catalog.getInfo()).isEqualTo("Catalog contains: \n");
	}
	
	@Test
	public void testGetInfo() {
		TypeOfManufactureStrategy industrialType = industrialWith(3, 5.0);
		ShoeStorageSystem airForce1 = createShoesWithoutDescription("AirForce1", 40.0, industrialType);
		ShoeStorageSystem nikeSportwear = createShoesWithoutDescription("B550", 40.0, industrialType);
		Catalog subCatalogue = createCatalog("Sub catalogue");
		
		catalog.addContents(airForce1);
		subCatalogue.addContents(nikeSportwear);
		catalog.addContents(subCatalogue);
		
		assertThat(catalog.getInfo())
							.isEqualTo("Catalog contains: \n"
										+ "Shoes name: AirForce1"
										+ "\n"
										+ "Sub catalogue contains: \n"
										+ "Shoes name: B550"
										+ "\n");
	}
	
	@Test
	public void testGetPriceWhenIsEmpty() {
		assertThat(catalog.getPrice()).isEqualTo(0.0);
	}
	
	@Test
	public void testGetPrice() {
		TypeOfManufactureStrategy industrialType = industrialWith(3, 5.0);
		ShoeStorageSystem airForce1 = createShoesWithoutDescription("AirForce1", 40.0, industrialType);
		ShoeStorageSystem nikeSportwear = createShoesWithoutDescription("B550", 40.0, industrialType);
		Catalog subCatalogue = createCatalog("Sub catalogue");
		
		catalog.addContents(airForce1);
		subCatalogue.addContents(nikeSportwear);
		catalog.addContents(subCatalogue);
		
		assertThat(catalog.getPrice()).isEqualTo(132.0);					
	}
}
