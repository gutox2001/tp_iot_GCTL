import { Text, View, StyleSheet } from 'react-native';

export default function Index() {
	return (
		<View style={styles.titleContainer}>
			<Text style={styles.text}>
				Edit app/index.tsx to edit this screen.
			</Text>
		</View>
	);
}

const styles = StyleSheet.create({
	titleContainer: {
		flex: 1,
		justifyContent: 'center',
		alignItems: 'center',
	},
	text: {
		fontSize: 20,
		fontWeight: 'bold',
	},
});
