package storagesystem.shoesdecorator;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Assertions.assertThatThrownBy;
import static storagesystem.price.strategy.IndustrialShoesStrategy.industrialWith;
import static storagesystem.structure.Catalog.createCatalog;
import static storagesystem.structure.Shoes.createShoesWithoutDescription;

import org.junit.Before;
import org.junit.Test;

import storagesystem.price.strategy.TypeOfManufactureStrategy;
import storagesystem.structure.Catalog;
import storagesystem.structure.ShoeStorageSystem;
import storagesystem.structure.Shoes;

public class ShoesDecoratorTesting {
	
	private Shoes shoes;
	
	@Before
	public void setup() {
		TypeOfManufactureStrategy industrialType = industrialWith(9, 15.0);
		shoes = createShoesWithoutDescription("Puma 1980", 70.0, industrialType);
	}

	@Test
	public void testComponentNull() {
		assertThatThrownBy(() -> new ShoesBrandInfoDecorator("Puma",null))
		.isInstanceOf(NullPointerException.class)
		.hasMessage("Component cannot be null");
	}
	
	@Test
	public void testAddBrandInfo() {
		ShoesInfo shoesWithBrand = new ShoesBrandInfoDecorator("Puma",shoes);
		
		assertThat(shoesWithBrand.getPrice()).isEqualTo(162.5);
		assertThat(shoesWithBrand.getInfo()).isEqualTo("Shoes name: Puma 1980"
														+ "\n"
														+ " of brand: Puma");
	}
	
	@Test
	public void testAddLogoInfo() {
		ShoesInfo shoesWithLogo = new ShoesLogoInfoDecorator("Puma logo",shoes);
		
		assertThat(shoesWithLogo.getPrice()).isEqualTo(155.0);
		assertThat(shoesWithLogo.getInfo()).isEqualTo("Shoes name: Puma 1980"
														+ "\n"
														+ " with logo: Puma logo");
	}
	
	@Test
	public void testAddSignatureInfo() {
		ShoesInfo shoesWithSignature = new ShoesSignatureInfoDecorator("Rudolf Dassler",shoes);
		
		assertThat(shoesWithSignature.getPrice()).isEqualTo(157.5);
		assertThat(shoesWithSignature.getInfo()).isEqualTo("Shoes name: Puma 1980"
															+ "\n"
															+ " signature: Rudolf Dassler");
	}
	
	@Test
	public void testAddBrandInfoAndLogoInfo() {
		ShoesInfo shoesDecorated = new ShoesBrandInfoDecorator("Puma",
									new ShoesLogoInfoDecorator("Puma logo",shoes));
		
		assertThat(shoesDecorated.getPrice()).isEqualTo(165.0);
		assertThat(shoesDecorated.getInfo()).isEqualTo("Shoes name: Puma 1980"
														+ "\n"
														+ " with logo: Puma logo"
														+ " of brand: Puma");
	}
	
	@Test
	public void testAddBrandSignatureAndLogoInfo() {
		ShoesInfo shoesDecorated = new ShoesBrandInfoDecorator("Puma",
									new ShoesSignatureInfoDecorator("Rudolf Dassler",
										new ShoesLogoInfoDecorator("Puma logo",shoes)));
		
		assertThat(shoesDecorated.getPrice()).isEqualTo(170.0);
		assertThat(shoesDecorated.getInfo()).isEqualTo("Shoes name: Puma 1980"
														+ "\n"
														+ " with logo: Puma logo"
														+ " signature: Rudolf Dassler"
														+ " of brand: Puma");
	}
	
	@Test
	public void testAggregateCatalog() {
		Catalog catalog = createCatalog("Catalog");
		TypeOfManufactureStrategy industrialType = industrialWith(3, 5.0);
		ShoeStorageSystem airForce1 = createShoesWithoutDescription("AirForce1", 40.0, industrialType);
		ShoesInfo shoesDecorated = new ShoesLogoInfoDecorator("Nike logo",
										new ShoesBrandInfoDecorator("Nike",
												createShoesWithoutDescription("B550",
																				40.0,
																				industrialType)));
		Catalog subCatalogue = createCatalog("Sub catalogue");
		
		catalog.addContents(airForce1);
		catalog.addContents(shoes);
		subCatalogue.addContents(shoesDecorated);
		catalog.addContents(subCatalogue);
		
		assertThat(catalog.getPrice()).isEqualTo(297.0);
		assertThat(catalog.getInfo()).isEqualTo("Catalog contains: \n"
												+ "Shoes name: AirForce1\n"
												+ "Shoes name: Puma 1980\n"
												+ "Sub catalogue contains: \n"
												+ "Shoes name: B550\n"
												+ " of brand: Nike with logo: Nike logo");
	}
	
	


}
